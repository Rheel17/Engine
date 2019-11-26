/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef COMPONENT_H_
#define COMPONENT_H_
#include "_common.h"

#include "Scene.h"
#include "Entity.h"
#include "Transform.h"
#include "UI/Input.h"
#include "UI/Elements/SceneElement.h"

namespace rheel {

class ComponentBase;

/**
 * A proxy class to handle player input
 */
class RE_API ComponentInputProxy {
	friend class ComponentBase;
	friend class SceneElement;

public:
	virtual ~ComponentInputProxy() = default;

	void ResetDeltas();

private:
	ComponentInputProxy(ComponentBase *component);

	void _OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void _OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void _OnCharacterInput(Input::Unicode character);
	void _OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods);
	void _OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods);
	void _OnMouseMove(const vec2& position);
	void _OnMouseJump(const vec2& position);
	void _OnMouseDrag(const vec2& origin, const vec2& position);
	void _OnMouseScroll(const vec2& scrollComponents);

	void _Register();
	void _Unregister();

	ComponentBase *_component;
	SceneElement *_source;

	std::unordered_set<Input::Key> _pressed_keys;
	std::unordered_set<Input::Scancode> _pressed_scancodes;
	vec2 _mouse_position;
	vec2 _mouse_delta;
	vec2 _mouse_scroll;
	bool _got_mouse = false;

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

	/**
	 * Called when a key on the keyboard is pressed down.
	 *
	 * Parameters
	 * 	key: the code of the key
	 * 	scancode: the location of the key on the keyboard
	 * 	mods: any modifiers that were used
	 */
	virtual void OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {}

	/**
	 * Called when a key on the keyboard is kept released.
	 *
	 * Parameters
	 * 	key: the code of the key
	 * 	scancode: the location of the key on the keyboard
	 * 	mods: any modifiers that were used
	 */
	virtual void OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {}

	/**
	 * Called when a text character is inputed.
	 *
	 * Parameters
	 * 	character: the unicode character that was inputed
	 */
	virtual void OnCharacterInput(Input::Unicode character) {}

	/**
	 * Called when a mouse button is pressed down.
	 *
	 * Parameters
	 * 	button: the button that was pressed
	 * 	mods: any modifiers that were used
	 */
	virtual void OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {}

	/**
	 * Called when a mouse button is released.
	 *
	 * Parameters
	 * 	button: the button that was pressed
	 * 	mods: any modifiers that were used
	 */
	virtual void OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {}

	/**
	 * Called when the mouse is moved without any buttons pressed.
	 *
	 * Parameters
	 * 	position: coordinates of the mouse position
	 */
	virtual void OnMouseMove(const vec2& position) {}

	/**
	 * Called when the mouse is jumped without the user specifically moving the
	 * mouse itself, but i.e. through a spring-back by re-enabling the cursor.
	 *
	 * Parameters
	 * 	position: coordinates of the mouse position
	 */
	virtual void OnMouseJump(const vec2& position) {}

	/**
	 * Called when the mouse is moved with at least one button pressed.
	 *
	 * Parameters
	 * 	origin: coordinates of the origin of the drag (where the mouse was
	 * 		clicked)
	 * 	position: coordinates of the mouse position
	 */
	virtual void OnMouseDrag(const vec2& origin, const vec2& position) {}

	/**
	 * Called when the mouse is scrolled.
	 *
	 * Parameters
	 * 	x: the x component of the scroll
	 * 	y: the y component of the scroll
	 */
	virtual void OnMouseScroll(const vec2& scrollComponents) {}

protected:
	ComponentBase() = default;

	float GetTime() const;
	float GetTimeDelta() const;
	bool IsKeyPressed(Input::Key key) const;
	bool IsKeyPressed(Input::Scancode scancode) const;
	const vec2& GetMousePosition() const;
	const vec2& GetMouseDelta() const;
	const vec2& GetMouseScroll() const;

private:
	Entity *_entity = nullptr;
	bool _receive_input = false;
	std::unique_ptr<ComponentInputProxy> _input_proxy;

	float _time = 0.0f;
	float _dt = 1.0f / 60.0f;

private:
	static const vec2 ZERO_VEC2;

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
