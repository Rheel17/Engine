/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_MULTICOMPONENTVIEW_INC
#define ENGINE_MULTICOMPONENTVIEW_INC

#ifndef ENGINE_REGISTRY_H
#error "Do not include directly, include via Registry.h"
#endif

#include <ranges>

namespace rheel::detail {

template<typename T>
struct single_iterator {
	using data_ptr = std::conditional_t<std::is_const_v<T>, const char*, char*>;

	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = value_type*;
	using reference = value_type&;
	using const_pointer = std::add_const_t<pointer>;
	using const_reference = std::add_const_t<reference>;

	single_iterator() = default;
	single_iterator(data_ptr data, std::size_t elem_sz) noexcept:
			_data(static_cast<data_ptr>(data)),
			_elem_sz(elem_sz) {}

	auto operator<=>(const single_iterator& iter) const noexcept {
		return _data <=> iter._data;
	}

	bool operator==(const single_iterator& iter) const noexcept = default;
	bool operator!=(const single_iterator& iter) const noexcept = default;

	reference operator*() { return *reinterpret_cast<pointer>(_data); }
	const_reference operator*() const { return *reinterpret_cast<const_pointer>(_data); }

	pointer operator->() { return reinterpret_cast<pointer>(_data); }
	const_pointer operator->() const { return reinterpret_cast<const_pointer>(_data); }

	single_iterator& operator++() noexcept {
		_data += _elem_sz;
		return *this;
	}

	single_iterator operator++(int) noexcept {
		single_iterator copy(*this);
		++(*this);
		return copy;
	}

	single_iterator& operator--() noexcept {
		_data -= _elem_sz;
		return *this;
	}

	single_iterator operator--(int) noexcept {
		single_iterator copy(*this);
		--(*this);
		return copy;
	}

	single_iterator& operator+=(difference_type n) noexcept {
		_data += _elem_sz * n;
		return *this;
	}

	single_iterator& operator-=(difference_type n) noexcept {
		_data -= _elem_sz * n;
		return *this;
	}

	single_iterator operator+(difference_type n) const noexcept {
		single_iterator copy(*this);
		copy += n;
		return copy;
	}

	friend single_iterator operator+(difference_type n, const single_iterator& iter) noexcept {
		return iter + n;
	}

	single_iterator operator-(difference_type n) const noexcept {
		single_iterator copy(*this);
		copy -= n;
		return copy;
	}

	difference_type operator-(single_iterator iter) const noexcept {
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

template<typename T>
struct single_view {
	using data_ptr = typename single_iterator<T>::data_ptr;

	data_ptr data;
	std::size_t element_size;
	std::size_t element_count;

	single_view() = default;

	template<typename Ci>
	single_view(std::span<Ci> span) :
			data(reinterpret_cast<data_ptr>(span.data())),
			element_size(sizeof(Ci)),
			element_count(span.size()) {}

	single_iterator<T> begin() const {
		return single_iterator<T>(data, element_size);
	}

	single_iterator<T> end() const {
		return single_iterator<T>(data + element_size * element_count, element_size);
	}
};

}

template<typename T>
[[maybe_unused]] constexpr const bool std::ranges::enable_borrowed_range<rheel::detail::single_view<T>> = true;

namespace rheel {

template<typename T, typename... C>
class CastedMultiComponentView {
	using data_ptr = std::conditional_t<std::is_const_v<T>, const char*, char*>;

public:
	CastedMultiComponentView() = default;

	explicit CastedMultiComponentView(Registry* registry) :
			_registry(registry),
			_views({ detail::single_view<T>(registry->GetComponents<C>())... }),
			_joined_range(_views | std::views::join) {}

	auto begin() const {
		return std::begin(_joined_range);
	}

	auto end() const {
		return std::end(_joined_range);
	}

	template<typename NewT>
		requires(std::is_base_of_v<NewT, C>&& ...)
	auto As() {
		return CastedMultiComponentView<std::conditional_t<std::is_const_v<T>, const NewT, NewT>, C...>(_registry);
	}

private:
	Registry* _registry;
	std::array<detail::single_view<T>, sizeof...(C)> _views;
	decltype(_views | std::views::join) _joined_range;

};

template<bool is_const, typename... C>
class MultiComponentView : public CastedMultiComponentView<std::conditional_t<is_const, const Component, Component>, C...> {

public:
	explicit MultiComponentView(Registry* registry) :
			CastedMultiComponentView<std::conditional_t<is_const, const Component, Component>, C...>(registry) {}
};

}

#endif
