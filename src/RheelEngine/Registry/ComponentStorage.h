/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_COMPONENTSTORAGE_H
#define ENGINE_COMPONENTSTORAGE_H
#include "../_common.h"

#include "../Component.h"

#include <span>

namespace rheel {

namespace detail {

template<typename F, typename Arg>
concept opt_function = requires(std::invoke_result_t<F, Arg> r) {
	{ r == std::nullopt } -> std::convertible_to<bool>;
	{ std::invoke_result_t<F, Arg>{ std::nullopt } };
};

template<typename F, typename Arg>
concept void_function = std::is_same_v<std::invoke_result_t<F, Arg>, void>;

template<typename F, typename Arg>
using forall_return = std::conditional_t<opt_function<F, Arg>, std::invoke_result_t<F, Arg>, void>;

}

class ComponentStorage {

public:
	ComponentStorage() = default;
	~ComponentStorage() {
		// delete all components in use
		static const auto deleter = [](Component& c) {
			c.~Component();
		};

		ForAll(deleter);

		// delete storage
		free(_storage); // NOLINT (malloc used, so need to free)
	}

	ComponentStorage(const ComponentStorage&) = delete;
	ComponentStorage& operator=(const ComponentStorage&) = delete;

	ComponentStorage(ComponentStorage&& cs) noexcept:
			_storage(cs._storage),
			_byte_capacity(cs._byte_capacity),
			_size(cs._size) {

		cs._storage = nullptr;
		cs._byte_capacity = 0;
		cs._size = 0;
	}

	ComponentStorage& operator=(ComponentStorage&&) noexcept = delete;

	template<std::invocable<Component&> F>
	detail::forall_return<F, Component&> ForAll(F f) {
		return _for_all<Component, ComponentStorage>(*this, f);
	}

	template<std::invocable<const Component&> F>
	detail::forall_return<F, const Component&> ForAll(F f) const {
		return _for_all<const Component, const ComponentStorage>(*this, f);
	}

	template<typename C>
	std::span<C> View() {
		return std::span<C>(static_cast<C*>(_storage), _size);
	}

	template<typename C>
	std::span<const C> View() const {
		return std::span<C>(static_cast<const C*>(_storage), _size);
	}

	template<typename C, typename... Args>
	C* NewInstance(Args&&... args) {
		// prepare the storage pointer
		_ensure_add_storage<C>();
		auto* c_storage = static_cast<C*>(_storage);

		// create the new component instance
		auto* ptr = new(c_storage + _size) C(std::forward<Args>(args)...);

		// initialize the component
		ptr->_index = _size++;
		ptr->_size = sizeof(C);

		return ptr;
	}

	// Returned: (entity_index, index_in_entity)
	template<typename C>
	std::pair<std::size_t, std::uint16_t> RemoveInstance(Entity* entities, std::size_t index) {
		auto* c_storage = static_cast<C*>(_storage);
		auto entity_index = c_storage[index]._entity_index;
		auto index_in_entity = c_storage[index]._index_in_entity;

		// find references to the to-be-swapped components
		Component** ptr_1 = &entities[entity_index]._components[index_in_entity];
		Component** ptr_2 = &entities[c_storage[_size - 1]._entity_index]._components[c_storage[_size - 1]._index_in_entity];

		// swap last and the to-remove index
		Component* tmp = *ptr_1;
		*ptr_1 = *ptr_2;
		*ptr_2 = tmp;

		std::swap(c_storage[index], c_storage[_size - 1]);
		c_storage[index]->_index = index;

		// element is now at last index, delete it
		_size--;
		auto* ptr = c_storage + _size;
		ptr->~C();

		return { entity_index, index_in_entity };
	}

private:
	template<typename C>
	void _ensure_add_storage() {
		if (_storage == nullptr) {
			// no storage allocated yet, so allocate for the first block
			_byte_capacity = sizeof(C);
			_storage = malloc(_byte_capacity); // NOLINT (malloc used for performance reasons)
			return;
		}

		if (_size * sizeof(C) < _byte_capacity) {
			// we have room for more, no need to reallocate anything
			return;
		}

		_byte_capacity *= 2;

		// Windows has _expand function, others don't.
#ifdef WIN32
		void* expanded_ptr = _expand(_storage, _byte_capacity);
		if (expanded_ptr == _storage) {
			// Expanded pointer is in the same location, but is now bigger. No
			// moves needed.
			return;
		}
#endif

		// allocate new storage
		void* new_storage = malloc(_byte_capacity); // NOLINT (malloc used for performance reasons)
		C* new_c_storage = static_cast<C*>(new_storage);
		C* old_c_storage = static_cast<C*>(_storage);

		// move containers to new storage
		for (std::size_t i = 0; i < _size; i++) {
			auto* entity = old_c_storage[i]._entity;
			auto index_in_entity = old_c_storage[i]._index_in_entity;

			new(new_c_storage + i) C(std::move(old_c_storage[i]));
			old_c_storage[i].~C();

			entity->_components[index_in_entity] = new_c_storage + i;
		}

		// free old storage and finish
		free(_storage); // NOLINT (malloc used, so need to free)
		_storage = new_storage;
	}

	template<typename C, typename Self, typename F>
		requires (detail::opt_function<F, C&> || detail::void_function<F, C&>)
	static detail::forall_return<F, C&> _for_all(Self& self, F f) {
		constexpr static bool is_const = std::is_const_v<Self>;

		using buffer_t = std::conditional_t<is_const, const char*, char*>;
		buffer_t ptr = static_cast<buffer_t>(self._storage);

		for (std::size_t i = 0; i < self._size; i++) {
			C* component = reinterpret_cast<C*>(ptr); // NOLINT (safe conversion)
			ptr += component->_size;

			if constexpr (detail::opt_function<F, C&>) {
				auto result = f(*component);
				if (result == std::nullopt) {
					continue;
				}

				return result;
			} else {
				f(*component);
			}
		}

		if constexpr (detail::opt_function<F, C&>) {
			return detail::forall_return<F, C&>{ std::nullopt };
		}
	}

	void* _storage = nullptr;
	std::size_t _byte_capacity = 0;
	std::size_t _size = 0;

};

}

#endif
