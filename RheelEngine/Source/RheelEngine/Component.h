/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef COMPONENT_H_
#define COMPONENT_H_
#include "_common.h"

#include "Scene.h"
#include "Entity.h"
#include "Transform.h"

namespace rheel {

class ComponentBase;

/**
 * A proxy class to handle player input
 */
class RE_API ComponentInputProxy {
	friend class ComponentBase;

private:
	ComponentInputProxy(ComponentBase *component);

	void _Register();
	void _Unregister();

	ComponentBase *_component;

};

/**
 * Base class of all components. If you want a custom component with a
 * transform, use either Component or RigidComponent as a base class.
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

	/**
	 * Returns the parent entity of this component
	 */
	const Entity *GetParent() const;

	/**
	 * Sets the receive input flag for this component. When this flag is set to
	 * true, the component will receive input events from the player.
	 */
	void SetReceiveInput(bool flag);

	/**
	 * Returns whether this component receives input from the player.
	 */
	bool ReceivesInput() const;

protected:
	ComponentBase() = default;

private:
	Entity *_entity = nullptr;
	bool _receive_input = false;
	std::unique_ptr<ComponentInputProxy> _input_proxy;

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

	mat4 CalculateAbsoluteTransformationMatrix() const {
		mat4 parentTransform = GetParent()->CalculateAbsoluteTransform().AsMatrix();
		mat4 thisTransform = transform.AsMatrix();
		return thisTransform * parentTransform;
	}

	_Transform CalculateAbsoluteTransform() const {
		return Transform(CalculateAbsoluteTransformationMatrix());
	}

	_Transform transform;

};

using Component = _Component<Transform>;
using RigidComponent = _Component<RigidTransform>;

}

#endif
