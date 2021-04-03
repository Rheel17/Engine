/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_COMPONENTSTORAGE_H
#define ENGINE_COMPONENTSTORAGE_H
#include "../_common.h"

#include "EntityStorage.h"
#include "../Component.h"

#include <span>

namespace rheel {

namespace detail {

template<typename Base, typename Derived>
concept HasBase = std::is_base_of_v<Base, Derived>;

}

class ComponentStorage {
	template<typename>
	friend
	class ComponentView;

public:
	ComponentStorage() = default;
	~ComponentStorage();

	ComponentStorage(const ComponentStorage&) = delete;
	ComponentStorage& operator=(const ComponentStorage&) = delete;

	ComponentStorage(ComponentStorage&& cs) noexcept;
	ComponentStorage& operator=(ComponentStorage&&) noexcept = delete;

	template<typename C, typename... Args>
	C* NewInstance(Args&& ... args) {
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

	// Returned: (entity pointer, index of component in entity's component list)
	template<typename C>
	std::pair<Entity*, std::uint16_t> RemoveInstance(const EntityStorage<Entity>& entities, std::size_t index) {
		auto* c_storage = static_cast<C*>(_storage);
		auto* entity = c_storage[index]._entity;
		auto index_in_entity = c_storage[index]._index_in_entity;

		// find references to the to-be-swapped components
		auto** ptr_1 = _component_pp(entity, index_in_entity);
		auto** ptr_2 = _component_pp(c_storage[_size - 1]._entity, c_storage[_size - 1]._index_in_entity);

		// swap last and the to-remove index
		Component* tmp = *ptr_1;
		*ptr_1 = *ptr_2;
		*ptr_2 = tmp;

		std::swap(c_storage[index], c_storage[_size - 1]);
		c_storage[index]._index = index;

		// element is now at last index, delete it
		_size--;
		auto* ptr = c_storage + _size;
		ptr->~C();

		return { entity, index_in_entity };
	}

	Component& operator[](std::size_t idx);
	const Component& operator[](std::size_t idx) const;

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

	Component** _component_pp(Entity* entity, std::size_t index_in_entity);

	void* _storage = nullptr;
	std::size_t _byte_capacity = 0;
	std::size_t _size = 0;

};

template<typename C>
class ComponentIterator {

public:
	using data_ptr = std::conditional_t<std::is_const_v<C>, const char*, char*>;
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = C;
	using pointer = value_type*;
	using reference = value_type&;
	using const_pointer = std::add_const_t<pointer>;
	using const_reference = std::add_const_t<reference>;

	ComponentIterator() = default;
	ComponentIterator(data_ptr data, std::size_t size) noexcept:
			_data(static_cast<data_ptr>(data)),
			_elem_sz(size) {}

	auto operator<=>(const ComponentIterator& iter) const noexcept {
		return _data <=> iter._data;
	}

	bool operator==(const ComponentIterator& iter) const noexcept = default;
	bool operator!=(const ComponentIterator& iter) const noexcept = default;

	reference operator*() { return *reinterpret_cast<pointer>(_data); }
	const_reference operator*() const { return *reinterpret_cast<const_pointer>(_data); }

	pointer operator->() { return reinterpret_cast<pointer>(_data); }
	const_pointer operator->() const { return reinterpret_cast<const_pointer>(_data); }

	ComponentIterator& operator++() noexcept {
		_data += _elem_sz;
		return *this;
	}

	ComponentIterator operator++(int) noexcept {
		ComponentIterator copy(*this);
		++(*this);
		return copy;
	}

	ComponentIterator& operator--() noexcept {
		_data -= _elem_sz;
		return *this;
	}

	ComponentIterator operator--(int) noexcept {
		ComponentIterator copy(*this);
		--(*this);
		return copy;
	}

	ComponentIterator& operator+=(difference_type n) noexcept {
		_data += _elem_sz * n;
		return *this;
	}

	ComponentIterator& operator-=(difference_type n) noexcept {
		_data -= _elem_sz * n;
		return *this;
	}

	ComponentIterator operator+(difference_type n) const noexcept {
		ComponentIterator copy(*this);
		copy += n;
		return copy;
	}

	friend ComponentIterator operator+(difference_type n, const ComponentIterator& iter) noexcept {
		return iter + n;
	}

	ComponentIterator operator-(difference_type n) const noexcept {
		ComponentIterator copy(*this);
		copy -= n;
		return copy;
	}

	difference_type operator-(ComponentIterator iter) const noexcept {
		return (_data - iter._data) / _elem_sz;
	}

	reference operator[](difference_type n) {
		return *reinterpret_cast<pointer>(_data + n * _elem_sz);
	}

	const_reference operator[](difference_type n) const {
		return *reinterpret_cast<const_pointer>(_data + n * _elem_sz);
	}

private:
	data_ptr _data;
	std::size_t _elem_sz;

};

template<typename C>
class ComponentView {
	using data_ptr = typename ComponentIterator<C>::data_ptr;
	using storage_t = std::conditional_t<std::is_const_v<C>, const ComponentStorage, ComponentStorage>;

	template<typename>
	friend class ComponentView;

public:
	ComponentView() = default;
	ComponentView(storage_t& storage) :
			_data(static_cast<data_ptr>(storage._storage)),
			_element_count(storage._size),
			_element_size(_element_count == 0
					? 1
					: static_cast<std::conditional_t<std::is_const_v<C>, const Component*, Component*>>(storage._storage)->_size) {}

	template<detail::HasBase<C> NewC>
	operator ComponentView<NewC>() requires (!std::is_const_v<C>) {
		return ComponentView<NewC>(_data, _element_count, _element_size);
	}

	template<detail::HasBase<C> NewC>
	operator ComponentView<const NewC>() const {
		return ComponentView(_data, _element_count, _element_size);
	}

	ComponentIterator<C> begin() const {
		return ComponentIterator<C>(_data, _element_size);
	}

	ComponentIterator<C> end() const {
		return ComponentIterator<C>(_data + _element_count * _element_size, _element_size);
	}

	std::size_t size() const {
		return _element_count;
	}

private:
	ComponentView(data_ptr data, std::size_t cnt, std::size_t sz) :
			_data(data),
			_element_count(cnt),
			_element_size(sz) {}

	data_ptr _data{};
	std::size_t _element_count{};
	std::size_t _element_size{};

};

}

template<typename C>
[[maybe_unused]] constexpr const bool std::ranges::enable_borrowed_range<rheel::ComponentView<C>> = true;

#endif
