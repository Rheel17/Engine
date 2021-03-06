/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_INPUTCALLBACK_H
#define RHEELENGINE_INPUTCALLBACK_H
#include "../_common.h"

#include "Input.h"

namespace rheel {

class RE_API InputCallback {

public:
	virtual ~InputCallback() = default;

	/**
	 * Called when this element has been resized.
	 */
	virtual void OnResize() {}

	/**
	 * Called when this element receives the focus in the UI.
	 */
	virtual void OnFocusGained() {}

	/**
	 * Called when the focus of this element is lost. This can happen because
	 * another element receives focus, or the parent window itself looses focus.
	 */
	virtual void OnFocusLost() {}

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
	 * Called when a key on the keyboard is kept pressed down long enough to
	 * trigger a repeat.
	 *
	 * Parameters
	 * 	key: the code of the key
	 * 	scancode: the location of the key on the keyboard
	 * 	mods: any modifiers that were used
	 */
	virtual void OnKeyRepeat(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {}

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
	 * Called when the mouse enters the bounds of this component.
	 *
	 * Parameters
	 * 	position: coordinates of the mouse position
	 */
	virtual void OnMouseEnter(const vec2& position) {}

	/**
	 * Called when the mouse exits the bounds of this component.
	 *
	 * Parameters
	 * 	position: coordinates of the mouse position
	 */
	virtual void OnMouseExit(const vec2& position) {}

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

};

}

#endif
