/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CACHE_H
#define RHEELENGINE_CACHE_H
#include "../_common.h"

#include <mutex>
#include <future>
#include <condition_variable>
#include <list>
#include <queue>

namespace rheel {

// TODO: possible error condition: capacity=n, >n threads put at the same time

struct cache_policy {
	virtual ~cache_policy() = default;

	/**
	 * called when an element is accessed
	 */
	virtual void Access(uintptr_t k) {}

	/**
	 * Called when an element is inserted
	 */
	virtual void Insert(uintptr_t k) {}

	/**
	 * Picks which element to remove
	 */
	virtual uintptr_t EnsureSpace() = 0;

	static constexpr uintptr_t DONT_REMOVE = 0;
};

/**
 * Simple policy where every element is kept for the duration of the cache
 */
struct keep_policy : public cache_policy {
	uintptr_t EnsureSpace() override {
		return DONT_REMOVE;
	}
};

/**
 * Policy where the least-recently used element is removed when there is no
 * space left
 */
struct least_recently_used_policy : public cache_policy {
	void Access(uintptr_t key) override {
		_lru_list.erase(_index[key]);
		Insert(key);
	}

	void Insert(uintptr_t key) override {
		_lru_list.push_back(key);
		_index[key] = --_lru_list.cend();
	}

	uintptr_t EnsureSpace() override {
		uintptr_t element = _lru_list.front();
		_lru_list.pop_front();
		_index.erase(element);
		return element;
	}

private:
	std::list<uintptr_t> _lru_list;
	std::unordered_map<uintptr_t, std::list<uintptr_t>::const_iterator> _index;

};

/**
 * Policy where the oldest element is removed when there is no space left
 */
struct last_in_frist_out_policy : public cache_policy {
	void Insert(uintptr_t key) override {
		_lifo_queue.push(key);
	}

	uintptr_t EnsureSpace() override {
		uintptr_t element = _lifo_queue.front();
		_lifo_queue.pop();
		return element;
	}

private:
	std::queue<uintptr_t> _lifo_queue;

};

/**
 * Thread-safe cache implementation with several policies. Because of the
 * internals of the cache, it works best and fastest if the key type (K) is
 * trivially-copyable.
 */
template<typename K, typename V, class Policy = keep_policy>
class RE_API Cache {

public:
	using SizeType = size_t;

	/**
	 * Constructs a cache with the maximum possible size
	 */
	Cache() :
			Cache(std::numeric_limits<SizeType>::max()) {

		static_assert(std::is_same_v<Policy, keep_policy>, "Default-constructed cache requires keep_policy");
	}

	/**
	 * Constructs a cache with a maximum number of elements.
	 */
	Cache(SizeType capacity) :
			_capacity(capacity) {}

	/**
	 * Returns the amount of elements in the cache, including the elements that
	 * are currently loading.
	 */
	SizeType GetSize() const {
		return _size;
	}

	/**
	 * Returns the capacity of the cache.
	 */
	SizeType GetCapacity() const {
		return _capacity;
	}

	/**
	 * Returns whether the cache contains the key.
	 */
	bool ContainsKey(const K& key) const {
		std::lock_guard lock(_mutex);
		return _key_set.find(key) != _key_set.end();
	}

	/**
	 * Returns the value for the given key. If the key is not in this cache,
	 * this causes undefined behaviour.
	 */
	const V& Get(const K& key) const {
		std::lock_guard lock(_mutex);

		auto element = reinterpret_cast<uintptr_t>(&(*_key_set.find(key)));
		_policy.Access(element);

		return _cache.find(element)->second;
	}

	/**
	 * Returns the value for the given key. If the key is not in this cache,
	 * this causes undefined behaviour.
	 */
	V& Get(const K& key) {
		std::lock_guard lock(_mutex);

		auto element = reinterpret_cast<uintptr_t>(&(*_key_set.find(key)));
		_policy.Access(element);

		return _cache.find(element)->second;
	}

	/**
	 * Atomically checks if a value for the key exists, and inserts a new value
	 * for the key if it does not exist. The value should be created by the
	 * constructor parameter. Returned is a reference to the (possibly newly
	 * inserted) reference and a bool denoting whether a new value was inserted.
	 *
	 * Depending on the cache policy, this might cause other elements to be
	 * removed from the cache.
	 */
	template<typename Constructor>
	V& Get(const K& key, Constructor constructor) {
		// ensure that no other thread is modifying the cache while the element
		// is loading/constructing. This is needed to ensure the element is
		// removed from the cache before it is accessed.
		std::lock_guard lock(_mutex);

		bool changed = Put(key, std::forward<Constructor>(constructor));
		auto element = reinterpret_cast<uintptr_t>(&(*_key_set.find(key)));

		if (!changed) {
			_policy.Access(element);
		}

		return _cache.find(element)->second;
	}

	/**
	 * Atomically checks if a value for the key exists, and inserts a new value
	 * for the key if it does not exist. The value will be created without
	 * holding the lock to the cache, so multiple Put() requests can work
	 * concurrently.
	 *
	 * Depending on the cache policy, this might cause other elements to be
	 * removed from the cache.
	 *
	 * Returns whether the element was newly inserted in the cache.
	 */
	template<typename Constructor>
	bool Put(const K& key, Constructor constructor) {
		uintptr_t pointer;

		// First check that the cache does not yet contain the key. Otherwise
		// check if it is already being loaded. If neither: add it to the
		// loading set and make space for the element.
		{
			std::unique_lock lock(_mutex);

			if (auto iter = _key_set.find(key); iter != _key_set.end()) {
				auto element = reinterpret_cast<uintptr_t>(&(*_key_set.find(key)));

				if (_cache.find(element) != _cache.end()) {
					return false;
				}

				if (auto iter2 = _loading.find(element); iter2 != _loading.end()) {
					iter2->second->wait(lock, [&]() { return _loading.find(element) == _loading.end(); });
					return false;
				}
			}

			// make space for the new element
			if (_size == _capacity) {
				uintptr_t remove = _policy.EnsureSpace();
				_cache.erase(remove);
				_key_set.erase(*reinterpret_cast<K*>(remove));
				_size--;
			}

			// initial insertion into the cache
			auto[iter, inserted] = _key_set.insert(key);
			pointer = reinterpret_cast<uintptr_t>(&(*iter));
			_size++;

			// insert the key into the loading map, and default-construct the
			// condition_variable.
			_loading[pointer] = std::make_unique<std::condition_variable_any>();
		}

		// construct the value
		V value = constructor(key);

		// Add the value to the cache, remove it from the loading set and notify
		// any waiting threads that the value has finished loading. Also notify
		// the cache policy that a new element has been inserted.
		{
			std::lock_guard lock(_mutex);

			// add the asset to the cache
			_cache.try_emplace(pointer, std::move(value));

			// remove the key from the loading set
			auto conditionVariable = std::move(_loading[pointer]);
			_loading.erase(pointer);

			// notify the policy of the insertion
			_policy.Insert(pointer);

			// notify waiting threads
			conditionVariable->notify_all();
		}

		return true;
	}

private:
	SizeType _size = 0;
	SizeType _capacity;

	std::unordered_set<K> _key_set;
	std::unordered_map<uintptr_t, V> _cache;
	std::unordered_map<uintptr_t, std::unique_ptr<std::condition_variable_any>> _loading;
	mutable std::recursive_mutex _mutex;

	Policy _policy;
};

}

#endif