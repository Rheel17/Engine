/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "InputComponent.h"

namespace rheel {

void InputComponent::ResetDeltas() {
	_mouse_delta = { 0.0f, 0.0f };
	_mouse_scroll = { 0.0f, 0.0f };
}

vec2 InputComponent::GetMousePosition() const {
	return _mouse_position;
}

vec2 InputComponent::GetMouseDelta() const {
	return _mouse_delta;
}

bool InputComponent::IsKeyPressed(Input::Key key) const {
	return _pressed_keys.contains(key);
}

bool InputComponent::IsKeyPressed(Input::Scancode scancode) const {
	return _pressed_scancodes.contains(scancode);
}

}
