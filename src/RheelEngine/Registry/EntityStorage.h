/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_ENTITYSTORAGE_H
#define ENGINE_ENTITYSTORAGE_H
#include "../_common.h"

#include <bit>
#include <cstdint>
#include <vector>

namespace rheel {

template<typename E>
class EntityStorage {
	static constexpr auto _bucket_memory_size_limit = 1024 * 1024;
	static constexpr auto _bucket_size = std::bit_floor(_bucket_memory_size_limit / sizeof(E));

	static constexpr std::size_t _bucket_shift = std::countr_zero(_bucket_size);
	static constexpr std::size_t _bucket_mask = _bucket_size - 1;

	struct bucket {
		std::array<bool, _bucket_size> occupied{};
		alignas(alignof(E)) std::byte* storage;

		bucket() :
				storage(new std::byte[_bucket_size * sizeof(E)]) {}

		~bucket() {
			if (storage == nullptr) {
				return;
			}

			E* data = reinterpret_cast<E*>(storage); // NOLINT (safe)

			for (std::size_t i = 0; i < _bucket_size; i++) {
				if (occupied[i]) {
					data[i].~E();
				}
			}

			delete[] storage;
		}

		RE_NO_COPY(bucket);

		bucket(bucket&& b) noexcept:
				occupied(b.occupied),
				storage(b.storage) {

			b.storage = nullptr;
		}

		bucket& operator=(bucket&& b) noexcept {
			if (this != &b) {
				this->~bucket();

				occupied = std::move(b.occupied);
				storage = b.storage;
				b.storage = nullptr;
			}

			return *this;
		}

		E* operator[](std::size_t index) {
			E* data = reinterpret_cast<E*>(storage); // NOLINT (safe)
			return data + index;
		}

		const E* operator[](std::size_t index) const {
			const E* data = reinterpret_cast<const E*>(storage); // NOLINT (safe)
			return data + index;
		}
	};

public:
	E* operator[](std::size_t index) {
#ifdef NDBEBUG
		return _buckets[index >> _bucket_shift][index & _bucket_mask];
#else
		auto bucket_index = index >> _bucket_shift;
		auto index_in_bucket = index & _bucket_mask;

		if (bucket_index >= _buckets.size()) {
			return nullptr;
		}

		auto& bucket = _buckets[bucket_index];
		if (bucket.occupied[index_in_bucket]) {
			return bucket[index_in_bucket];
		}

		return nullptr;
#endif
	}

	const E* operator[](std::size_t index) const {
#ifdef NDBEBUG
		return _buckets[index >> _bucket_shift][index & _bucket_mask];
#else
		auto bucket_index = index >> _bucket_shift;
		auto index_in_bucket = index & _bucket_mask;

		if (bucket_index >= _buckets.size()) {
			return nullptr;
		}

		auto& bucket = _buckets[bucket_index];
		if (bucket.occupied[index_in_bucket]) {
			return bucket[index_in_bucket];
		}

		return nullptr;
#endif
	}

	template<typename... Args>
	std::pair<E&, std::size_t> Add(Args&&... args) {
		std::size_t index = _first_unused_index;
		std::size_t bucket_index = index >> _bucket_shift;
		std::size_t in_bucket = index & _bucket_mask;

		if (bucket_index >= _buckets.size()) {
			_buckets.resize(bucket_index + 1);
		}

		auto& bucket = _buckets[bucket_index];
		E* pointer = bucket[in_bucket];

		new(pointer) E(std::forward<Args>(args)...);
		bucket.occupied[in_bucket] = true;

		_find_next_unused_index();
		return { *pointer, index };
	}

	void Remove(std::size_t index) {
		auto& bucket = _buckets[index >> _bucket_shift];
		std::size_t in_bucket = index & _bucket_mask;

		// call destructor
		E* pointer = bucket[in_bucket];
		pointer->~E();

		// reset unused indices
		if (index < _first_unused_index) {
			_first_unused_index = index;
		}

		// set free
		bucket.occupied[in_bucket] = false;
	}

private:
	void _find_next_unused_index() {
		for (;; _first_unused_index++) {
			if ((_first_unused_index >> _bucket_shift) >= _buckets.size()) {
				break;
			}

			if (!_buckets[_first_unused_index >> _bucket_shift].occupied[_first_unused_index & _bucket_mask]) {
				break;
			}
		}
	}

	std::vector<bucket> _buckets;
	std::size_t _first_unused_index = 0;

};

}

#endif
