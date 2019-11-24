/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Component.h"

namespace rheel {

ComponentInputProxy::ComponentInputProxy(ComponentBase *component) :
		_component(component) {}


void ComponentInputProxy::OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {

}

void ComponentInputProxy::OnKeyRepeat(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {

}

void ComponentInputProxy::OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {

}

void ComponentInputProxy::OnCharacterInput(wchar_t character) {

}

void ComponentInputProxy::OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {

}

void ComponentInputProxy::OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {

}

void ComponentInputProxy::OnMouseEnter(const vec2& position) {

}

void ComponentInputProxy::OnMouseExit(const vec2& position) {

}

void ComponentInputProxy::OnMouseMove(const vec2& position) {

}

void ComponentInputProxy::OnMouseJump(const vec2& position) {

}

void ComponentInputProxy::OnMouseDrag(const vec2& origin, const vec2& position) {

}

void ComponentInputProxy::OnMouseScroll(const vec2& scrollComponents) {

}

void ComponentInputProxy::_Register() {
	_component->GetParent()->scene->_input_components.push_back(this);
}

void ComponentInputProxy::_Unregister() {
	auto& vec = _component->GetParent()->scene->_input_components;
	vec.erase(std::find(vec.begin(), vec.end(), this));
}

Entity *ComponentBase::GetParent() {
	return _entity;
}

const Entity *ComponentBase::GetParent() const {
	return _entity;
}

void ComponentBase::SetReceiveInput(bool flag) {
	if (_receive_input == flag) {
		return;
	}

	if (!flag) {
		_input_proxy->_Unregister();
	} else {
		if (!_input_proxy) {
			_input_proxy = std::unique_ptr<ComponentInputProxy>(new ComponentInputProxy(this));
		}

		_input_proxy->_Register();
	}

	_receive_input = flag;
}

bool ComponentBase::ReceivesInput() const {
	return _receive_input;
}

}
