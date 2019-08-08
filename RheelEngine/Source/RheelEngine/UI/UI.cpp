#include "UI.h"

namespace rheel {

UI::UI() :
		_ui_container(new Container) {

	_ui_container->_parent_ui = this;
}

UI::~UI() {
	delete _ui_container;
}

Container *UI::GetContainer() {
	return _ui_container;
}

void UI::Draw() const {
	_ui_container->Draw();
}

std::shared_ptr<UI> UI::Create() {
	return std::shared_ptr<UI>(new UI());
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
