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
		DrawStandardButton_(color_foreground_disabled, color_background_disabled);
		return;
	}

	if (_active) {
		DrawStandardButton_(color_foreground_active, color_background_active);
		return;
	}

	if (_rollover) {
		DrawStandardButton_(color_foreground_rollover, color_background_rollover);
		return;
	}

	DrawStandardButton_(color_foreground, color_background);
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

void ButtonElement::DrawStandardButton_(Color foreground, Color background) const {
	DrawColoredQuad(GetBounds(), background);
}

}