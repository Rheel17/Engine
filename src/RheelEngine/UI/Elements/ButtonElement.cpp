/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "ButtonElement.h"

namespace rheel {

ButtonElement::ButtonElement() :
		text("Button"),
		_action() {

	SetFocusable(true);
}

ButtonElement::ButtonElement(const std::string& text) :
		text(text),
		_action() {

	SetFocusable(true);
}

ButtonElement::ButtonElement(std::string&& text) :
		text(std::move(text)),
		_action() {

	SetFocusable(true);
}

void ButtonElement::PerformAction() const {
	_action();
}

void ButtonElement::DoDraw(float time, float dt) const {
	if (!_enabled) {
		_draw_standard_button(color_foreground_disabled, color_background_disabled);
		return;
	}

	if (_active) {
		_draw_standard_button(color_foreground_active, color_background_active);
		return;
	}

	if (_rollover) {
		_draw_standard_button(color_foreground_rollover, color_background_rollover);
		return;
	}

	_draw_standard_button(color_foreground, color_background);
}

void ButtonElement::OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {
	RequestFocus();
	_active = true;
}

void ButtonElement::OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {
	_active = false;
	PerformAction();
}

void ButtonElement::OnMouseEnter(const vec2& position) {
	_rollover = true;
}

void ButtonElement::OnMouseExit(const vec2& position) {
	_rollover = false;
	_active = false;
}

void ButtonElement::_draw_standard_button(Color foreground, Color background) const {
	DrawColoredQuad(GetBounds(), background);
}

}