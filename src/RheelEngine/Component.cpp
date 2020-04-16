/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Component.h"

namespace rheel {

const vec2 ComponentBase::_zero_vec_2(0.0f, 0.0f);

ComponentInputProxy::ComponentInputProxy(ComponentBase* component) :
		_component(component),
		_source(nullptr) {}

void ComponentInputProxy::ResetDeltas() {
	_mouse_delta = { 0.0f, 0.0f };
	_mouse_scroll = { 0.0f, 0.0f };
}

void ComponentInputProxy::OnKeyPress_(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	_pressed_keys.insert(key);
	_pressed_scancodes.insert(scancode);
	_component->OnKeyPress(key, scancode, mods);
}

void ComponentInputProxy::OnKeyRelease_(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	_pressed_keys.erase(key);
	_pressed_scancodes.erase(scancode);
	_component->OnKeyRelease(key, scancode, mods);
}

void ComponentInputProxy::OnCharacterInput_(Input::Unicode character) {
	_component->OnCharacterInput(character);
}

void ComponentInputProxy::OnMouseButtonPress_(Input::MouseButton button, Input::Modifiers mods) {
	_component->OnMouseButtonPress(button, mods);
}

void ComponentInputProxy::OnMouseButtonRelease_(Input::MouseButton button, Input::Modifiers mods) {
	_component->OnMouseButtonRelease(button, mods);
}

void ComponentInputProxy::OnMouseMove_(const vec2& position) {
	if (_got_mouse) {
		_mouse_delta = position - _mouse_position;
	}

	_mouse_position = position;
	_got_mouse = true;

	_component->OnMouseMove(position);
}

void ComponentInputProxy::OnMouseJump_(const vec2& position) {
	_mouse_position = position;
	_got_mouse = true;

	_component->OnMouseJump(position);
}

void ComponentInputProxy::OnMouseDrag_(const vec2& origin, const vec2& position) {
	if (_got_mouse) {
		_mouse_delta = position - _mouse_position;
	}

	_mouse_position = position;
	_got_mouse = true;

	_component->OnMouseDrag(origin, position);
}

void ComponentInputProxy::OnMouseScroll_(const vec2& scrollComponents) {
	_mouse_scroll = scrollComponents;
	_component->OnMouseScroll(scrollComponents);
}

void ComponentInputProxy::Register_() {
	_component->GetParent()->scene->_input_components.push_back(this);
}

void ComponentInputProxy::Unregister_() {
	auto& vec = _component->GetParent()->scene->_input_components;
	vec.erase(std::find(vec.begin(), vec.end(), this));
}

Entity* ComponentBase::GetParent() {
	return _entity;
}

const Entity* ComponentBase::GetParent() const {
	return _entity;
}

void ComponentBase::SetReceiveInput(bool flag) {
	if (_receive_input == flag) {
		return;
	}

	if (!flag) {
		_input_proxy->Unregister_();
	} else {
		if (!_input_proxy) {
			_input_proxy = std::unique_ptr<ComponentInputProxy>(new ComponentInputProxy(this));
		}

		_input_proxy->Register_();
	}

	_receive_input = flag;
}

bool ComponentBase::ReceivesInput() const {
	return _receive_input;
}

float ComponentBase::GetTime() const {
	return _time;
}

float ComponentBase::GetTimeDelta() const {
	return _dt;
}

bool ComponentBase::IsKeyPressed(Input::Key key) const {
	if (_receive_input) {
		return _input_proxy->_pressed_keys.find(key) != _input_proxy->_pressed_keys.end();
	}

	Log::Error() << "Component not registered to receive input events" << std::endl;
	return false;
}

bool ComponentBase::IsKeyPressed(Input::Scancode scancode) const {
	if (_receive_input) {
		return _input_proxy->_pressed_scancodes.find(scancode) != _input_proxy->_pressed_scancodes.end();
	}

	Log::Error() << "Component not registered to receive input events" << std::endl;
	return false;
}

const vec2& ComponentBase::GetMousePosition() const {
	if (_receive_input) {
		return _input_proxy->_mouse_position;
	}

	Log::Error() << "Component not registered to receive input events" << std::endl;
	return _zero_vec_2;
}

const vec2& ComponentBase::GetMouseDelta() const {
	if (_receive_input) {
		return _input_proxy->_mouse_delta;
	}

	Log::Error() << "Component not registered to receive input events" << std::endl;
	return _zero_vec_2;
}

const vec2& ComponentBase::GetMouseScroll() const {
	if (_receive_input) {
		return _input_proxy->_mouse_scroll;
	}

	Log::Error() << "Component not registered to receive input events" << std::endl;
	return _zero_vec_2;
}

}
