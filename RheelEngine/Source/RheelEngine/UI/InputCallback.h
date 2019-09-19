#ifndef INPUTCALLBACK_H_
#define INPUTCALLBACK_H_
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
	virtual void OnCharacterInput(wchar_t character) {}

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
	 * x: the x-coordinate of the mouse position
	 * y: the y-coordinate of the mouse position
	 */
	virtual void OnMouseEnter(const vec2& position) {}

	/**
	 * Called when the mouse exits the bounds of this component.
	 *
	 * x: the x-coordinate of the mouse position
	 * y: the y-coordinate of the mouse position
	 */
	virtual void OnMouseExit(const vec2& position) {}

	/**
	 * Called when the mouse is moved without any buttons pressed.
	 *
	 * Parameters
	 * 	x: the x-coordinate of the new mouse position
	 * 	y: the y-coordinate of the new mouse position
	 */
	virtual void OnMouseMove(const vec2& position) {}

	/**
	 * Called when the mouse is moved with at least one button pressed.
	 *
	 * Parameters
	 * 	xOrigin: the x-coordinate of the origin of the drag (where the mouse was
	 * 		clicked)
	 * 	yOrigin: the y-coordinate of the origin of the drag (where the mouse was
	 * 		clicked)
	 * 	x: the x-coordinate of the current mouse position
	 * 	y: the y-coordinate of the current mouse position
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

};

}

#endif
