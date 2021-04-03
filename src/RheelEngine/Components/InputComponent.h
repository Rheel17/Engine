/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_INPUTCOMPONENT_H
#define ENGINE_INPUTCOMPONENT_H
#include "../_common.h"

#include "../Component.h"
#include "../UI/Input.h"

namespace rheel {

class RE_API InputComponent : public Component {
	friend class SceneElement;

public:
	void ResetDeltas();

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
	virtual void OnMouseScroll(const vec2& scroll_components) {}

	vec2 GetMousePosition() const;
	vec2 GetMouseDelta() const;

	bool IsKeyPressed(Input::Key key) const;
	bool IsKeyPressed(Input::Scancode scancode) const;

private:
	std::set<Input::Key> _pressed_keys;
	std::set<Input::Scancode> _pressed_scancodes;
	vec2 _mouse_position{};
	vec2 _mouse_delta{};
	vec2 _mouse_scroll{};
	bool _got_mouse{};

};

}

#endif
