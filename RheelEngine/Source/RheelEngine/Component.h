/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef COMPONENT_H_
#define COMPONENT_H_
#include "_common.h"

#include "Entity.h"
#include "Transform.h"

namespace rheel {

/**
 * Base class of all components. Do not use this class as a direct base class;
 * use Component or RigidComponent instead.
 */
class RE_API ComponentBase {
	RE_NO_MOVE(ComponentBase);
	RE_NO_COPY(ComponentBase);

	template<typename _Transform>
	friend class _Component;
	friend class Entity;

public:
	virtual ~ComponentBase() = default;

	/**
	 * Called when the component is activated; either when the component is
	 * added to an active entity, or when the entity it belongs to is activated
	 * itself.
	 */
	virtual void Activate() {}

	/**
	 * Called when the component is deactivated; either when the component is
	 * removed from an active entity, or when the entity is removed from the
	 * scene.
	 */
	virtual void Deactivate() {}

	/**
	 * Called in the update cycle.
	 */
	virtual void Update() {}

	/**
	 * Called before the scene is rendered, can be used to setup render
	 * parameters
	 */
	virtual void Render() {}

	/**
	 * Returns the parent entity of this component
	 */
	Entity *GetParent();

private:
	ComponentBase() = default;

	Entity _entity;

};

/**
 * General base class for components. Most commonly, you want to use Component
 * or RigidComponent as actual base classes for custom components.
 */
template<typename _Transform>
class RE_API _Component : public ComponentBase {
	static_assert(std::is_base_of_v<RigidTransform, _Transform>, "Component transform must extend RigidTransform");

public:
	virtual ~_Component() = default;

	_Transform transform;

};

using Component = _Component<Transform>;
using RigidComponent = _Component<RigidTransform>;

}

#endif
