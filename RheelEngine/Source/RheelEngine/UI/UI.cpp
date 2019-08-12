#include "UI.h"

namespace rheel {

UI::UI(unsigned width, unsigned height) :
		_width(width), _height(height) {
	_ui_container._parent_ui = this;
}

void UI::SetContainer(Container&& container) {
	_ui_container = std::move(container);
	_ui_container.SetBounds({ 0, 0, _width, _height });
}

const Container& UI::GetContainer() const {
	return _ui_container;
}

Element *UI::ElementAt(unsigned x, unsigned y) {
	return _ui_container.ElementAt(x, y);
}

void UI::Draw() const {
	_ui_container.Draw();
}

void UI::OnKey(Input::Key key, Input::Scancode scancode, Input::Action action, Input::Modifiers mods) {

}

void UI::OnCharacter(wchar_t character) {

}

void UI::OnMouseMove(float x, float y) {
	_mouse.x = x;
	_mouse.y = y;
}

void UI::OnMouseButton(Input::MouseButton button, Input::Action action, Input::Modifiers mods) {

}

void UI::OnScroll(float x, float y) {

}

}
