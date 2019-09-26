/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "ScriptInput.h"

namespace rheel {

ScriptInput::~ScriptInput() {}

SceneElement *ScriptInput::InputSource() const {
	return _source_element;
}

const vec2& ScriptInput::InputMousePosition() const {
	return _mouse;
}

const vec2& ScriptInput::InputMouseDelta() const {
	return _mouse_delta;
}

const vec2& ScriptInput::InputScroll() const {
	return _scroll;
}

bool ScriptInput::IsButtonDown(Input::MouseButton button) const {
	return _pressed_mouse_buttons.find(button) != _pressed_mouse_buttons.end();
}

bool ScriptInput::IsKeyPressed(Input::Key key) const {
	return _pressed_keys.find(key) != _pressed_keys.end();
}

bool ScriptInput::IsKeyPressed(Input::Scancode scancode) const {
	return _pressed_scancodes.find(scancode) != _pressed_scancodes.end();
}

void ScriptInput::_OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	_pressed_keys.insert(key);
	_pressed_scancodes.insert(scancode);

	OnKeyPress(key, scancode, mods);
}

void ScriptInput::_OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	_pressed_keys.erase(key);
	_pressed_scancodes.erase(scancode);

	OnKeyRelease(key, scancode, mods);
}

void ScriptInput::_OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {
	_pressed_mouse_buttons.insert(button);

	OnMouseButtonPress(button, mods);
}

void ScriptInput::_OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {
	_pressed_mouse_buttons.erase(button);

	OnMouseButtonRelease(button, mods);
}

void ScriptInput::_OnMouseMove(const vec2& position) {
	if (_got_mouse) {
		_mouse_delta = position - _mouse;
	}

	_mouse = position;
	_got_mouse = true;

	OnMouseMove(position);
}

void ScriptInput::_OnMouseJump(const vec2& position) {
	_mouse = position;
	_got_mouse = true;
}

void ScriptInput::_OnMouseScroll(const vec2& scrollComponents) {
	_scroll = scrollComponents;

	OnMouseScroll(scrollComponents);
}

void ScriptInput::_ResetDeltas() {
	_mouse_delta = { 0, 0 };
	_scroll = { 0, 0 };
}

}
