#include "UI.h"

namespace rheel {

UI::UI(unsigned width, unsigned height) :
		_width(width), _height(height) {
	_ui_container._parent_ui = this;
}

void UI::SetContainer(const Container& container) {
	_ui_container = container;
	_ui_container.SetBounds({ 0, 0, _width, _height });
}

const Container& UI::GetContainer() const {
	return _ui_container;
}

void UI::Draw() const {
	_ui_container.Draw();
}

void UI::OnKey(Input::Key key, Input::Scancode scancode, Input::Action action, Input::Modifiers mods) {

}

void UI::OnCharacter(wchar_t character) {

}

void UI::OnMouseMove(float x, float y) {

}

void UI::OnMouseButton(Input::MouseButton button, Input::Action action, Input::Modifiers mods) {

}

void UI::OnScroll(float x, float y) {

}

}
