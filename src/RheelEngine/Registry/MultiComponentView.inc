/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_MULTICOMPONENTVIEW_INC
#define ENGINE_MULTICOMPONENTVIEW_INC

#ifndef ENGINE_REGISTRY_H
#error "Do not include directly, include via Registry.h"
#endif


namespace rheel {

template<typename R, typename T, typename... C>
class CastedMultiComponentView {

public:
	CastedMultiComponentView() = default;

	explicit CastedMultiComponentView(R* registry) :
			_registry(registry),
			_views({ registry->template GetComponents<C>()... }),
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
		return CastedMultiComponentView<R, std::conditional_t<std::is_const_v<T>, const NewT, NewT>, C...>(_registry);
	}

private:
	R* _registry;
	std::array<ComponentView<T>, sizeof...(C)> _views;
	decltype(_views | std::views::join) _joined_range;

};

template<typename R, typename... C>
class MultiComponentView : public CastedMultiComponentView<R, std::conditional_t<std::is_const_v<R>, const Component, Component>, C...> {

public:
	explicit MultiComponentView(R* registry) :
			CastedMultiComponentView<R, std::conditional_t<std::is_const_v<R>, const Component, Component>, C...>(registry) {}
};

}

#endif
