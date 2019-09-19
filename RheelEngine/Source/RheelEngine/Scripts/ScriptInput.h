#ifndef SCRIPTINPUT_H_
#define SCRIPTINPUT_H_
#include "../_common.h"

#include "../UI/Input.h"

#include <unordered_set>

namespace rheel {

/**
 * A general base class for scripts that require mouse/keyboard input
 */
class RE_API ScriptInput {
	friend class Scene;
	friend class SceneElement;

public:
	virtual ~ScriptInput();

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
	virtual void OnMouseMove(const vec2& position) {}

	/**
	 * Receives the input events from a SceneElement showing the parent scene.
	 */
	virtual void OnMouseScroll(const vec2& scrollComponents) {}

private:
	// pass-throughs from SceneElement
	void _OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void _OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void _OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods);
	void _OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods);
	void _OnMouseMove(const vec2& position);
	void _OnMouseScroll(const vec2& scrollComponents);
	void _ResetDeltas();

	bool _got_mouse = false;
	vec2 _mouse;
	vec2 _mouse_delta;
	vec2 _scroll;

	std::unordered_set<Input::MouseButton> _pressed_mouse_buttons;
	std::unordered_set<Input::Key> _pressed_keys;
	std::unordered_set<Input::Scancode> _pressed_scancodes;

};

}

#endif
