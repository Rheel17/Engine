#ifndef INPUTSCRIPT_H_
#define INPUTSCRIPT_H_

#include "../UI/Input.h"

#include <set>

namespace rheel {

/**
 * A general base class for scripts that require mouse/keyboard input
 */
class InputScript {
	friend class SceneElement;
	friend class Scene;

public:
	virtual ~InputScript();

protected:
	/**
	 * Returns the current mouse position
	 */
	const vec2& InputMousePosition() const;

	/**
	 * Returns the mouse movement of the last frame
	 */
	const vec2& InputMouseDelta() const;

	/**
	 * Returns the scroll wheel movement of the last frame
	 */
	const vec2& InputScroll() const;

	/**
	 * Returns whether the specified mouse button is pressed down.
	 */
	bool IsButtonDown(Input::MouseButton button) const;

	/**
	 * Returns whether the specified key is pressed down, where the key is
	 * specified as a key code.
	 */
	bool IsKeyPressed(Input::Key key) const;

	/**
	 * Returns whether the specified key is pressed down, where the key is
	 * specified as a scancode.
	 */
	bool IsKeyPressed(Input::Scancode scancode) const;

	/**
	 * Receives the input events from a SceneElement showing the parent scene.
	 */
	virtual void OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {}

	/**
	 * Receives the input events from a SceneElement showing the parent scene.
	 */
	virtual void OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {}

	/**
	 * Receives the input events from a SceneElement showing the parent scene.
	 */
	virtual void OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {}

	/**
	 * Receives the input events from a SceneElement showing the parent scene.
	 */
	virtual void OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {}

	/**
	 * Receives the input events from a SceneElement showing the parent scene.
	 */
	virtual void OnMouseMove(float x, float y) {}

	/**
	 * Receives the input events from a SceneElement showing the parent scene.
	 */
	virtual void OnMouseScroll(float x, float y) {}

private:
	// pass-throughs from SceneElement
	void _OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void _OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void _OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods);
	void _OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods);
	void _OnMouseMove(float x, float y);
	void _OnMouseScroll(float x, float y);
	void _ResetDeltas();

	vec2 _mouse;
	vec2 _mouse_delta;
	vec2 _scroll;

	std::set<Input::MouseButton> _pressed_mouse_buttons;
	std::set<Input::Key> _pressed_keys;
	std::set<Input::Scancode> _pressed_scancodes;

};

}

#endif
