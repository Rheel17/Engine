#include "InputScript.h"

namespace rheel {

InputScript::~InputScript() {}

const vec2& InputScript::InputMousePosition() const {
	return _mouse;
}

const vec2& InputScript::InputMouseDelta() const {
	return _mouse_delta;
}

const vec2& InputScript::InputScroll() const {
	return _scroll;
}

bool InputScript::IsButtonDown(Input::MouseButton button) const {
	return _pressed_mouse_buttons.find(button) != _pressed_mouse_buttons.end();
}

bool InputScript::IsKeyPressed(Input::Key key) const {
	return _pressed_keys.find(key) != _pressed_keys.end();
}

bool InputScript::IsKeyPressed(Input::Scancode scancode) const {
	return _pressed_scancodes.find(scancode) != _pressed_scancodes.end();
}

void InputScript::_OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	_pressed_keys.insert(key);
	_pressed_scancodes.insert(scancode);

	OnKeyPress(key, scancode, mods);
}

void InputScript::_OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	_pressed_keys.erase(key);
	_pressed_scancodes.erase(scancode);

	OnKeyRelease(key, scancode, mods);
}

void InputScript::_OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {
	_pressed_mouse_buttons.insert(button);

	OnMouseButtonPress(button, mods);
}

void InputScript::_OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {
	_pressed_mouse_buttons.erase(button);

	OnMouseButtonRelease(button, mods);
}

void InputScript::_OnMouseMove(float x, float y) {
	vec2 newPosition = { x, y };
	_mouse_delta = newPosition - _mouse;
	_mouse = newPosition;

	OnMouseMove(x, y);
}

void InputScript::_OnMouseScroll(float x, float y) {
	_scroll = { x, y };

	OnMouseScroll(x, y);
}

void InputScript::_ResetDeltas() {
	_mouse_delta = { 0, 0 };
	_scroll = { 0, 0 };
}

}
