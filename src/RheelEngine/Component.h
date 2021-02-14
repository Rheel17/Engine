/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_COMPONENT_H
#define RHEELENGINE_COMPONENT_H
#include "_common.h"

namespace rheel {

class Entity;

using ComponentId = std::uint16_t;

enum class ComponentFlags : std::uint64_t {
	BUILTIN = 1
};

inline ComponentFlags operator|(ComponentFlags a, ComponentFlags b) {
	return static_cast<ComponentFlags>(
			static_cast<std::underlying_type_t<ComponentFlags>>(a) | static_cast<std::underlying_type_t<ComponentFlags>>(b)
	);
}

/**
 * Base class of all components.
 */
class RE_API Component {
	friend class Entity;
	friend class ComponentStorage;
	friend class Registry;
	friend class Scene;

public:
	virtual ~Component() = default;

	RE_NO_COPY(Component);

	Component(Component&& c) noexcept;
	Component& operator=(Component&& c) noexcept;

	/**
	 * Called in the update cycle.
	 */
	virtual void Update() {}

	/**
	 * Returns the parent entity of this component
	 */
	Entity& GetEntity();

	/**
	 * Returns the parent entity of this component
	 */
	const Entity& GetEntity() const;

	/**
	 * Activates the component, i.e. the next time this component could be
	 * updated, it will be. Either this frame or the next.
	 *
	 * @see Deactivate()
	 */
	void Activate();

	/**
	 * Deactivates the component, i.e. the next time this component would have
	 * been updated, it won't be.
	 *
	 * @see Activate()
	 */
	void Deactivate();

protected:
	Component() = default;

	virtual void OnActivate() {}
	virtual void OnDeactivate() {}

	const float& time = _time;
	const float& dt = _dt;

private:
	Entity* _entity{};
	std::size_t _index{};
	std::size_t _size{};
	std::uint16_t _index_in_entity{};

	float _time = 0.0f;
	float _dt = 1.0f / 60.0f;

};

template<typename C>
concept HasComponentId = requires {
	{ C::id } -> std::same_as<ComponentId>;
};

template<typename C>
concept ComponentClass = std::is_base_of_v<Component, C> && std::is_nothrow_move_constructible_v<C> && HasComponentId<C>;

template<typename C>
concept FlaggedComponent = ComponentClass<C> && requires {
	{ C::flags } -> std::same_as<ComponentFlags>;
};

template<typename C, ComponentFlags Flag>
concept ComponentWithFlag = FlaggedComponent<C> && (C::flags & Flag);

}

#endif
