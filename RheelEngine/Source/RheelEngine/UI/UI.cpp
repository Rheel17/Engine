#include "UI.h"

namespace rheel {

UI::UI(unsigned width, unsigned height) :
		_width(width), _height(height) {}

void UI::SetContainer(Container&& container) {
	_ui_container = std::move(container);
	_ui_container._parent_ui = this;
	_ui_container.SetBounds({ 0, 0, _width, _height });
}

const Container& UI::GetContainer() const {
	return _ui_container;
}

Element *UI::ElementAt(unsigned x, unsigned y) {
	return _ui_container.ElementAt(x, y);
}

void UI::RequestFocus(Element *element) {
	if (_focus_element == element || !element->IsFocusable()) {
		return;
	}

	if (_focus_element && _focus) {
		_focus_element->OnFocusLost();
	}

	_focus_element = element;

	if (_focus_element && _focus) {
		_focus_element->OnFocusGained();
	}
}

Element *UI::FocusElement() const {
	return _focus_element;
}

void UI::Draw() const {
	_ui_container.Draw();
}

void UI::OnKey(Input::Key key, Input::Scancode scancode, Input::Action action, Input::Modifiers mods) {
	if (_focus_element) {
		switch (action) {
			case Input::Action::PRESS:
				_focus_element->OnKeyPress(key, scancode, mods);
				break;
			case Input::Action::REPEAT:
				_focus_element->OnKeyRepeat(key, scancode, mods);
				break;
			case Input::Action::RELEASE:
				_focus_element->OnKeyRelease(key, scancode, mods);
				break;
		}
	}
}

void UI::OnCharacter(wchar_t character) {
	if (_focus_element) {
		_focus_element->OnCharacterInput(character);
	}
}

void UI::OnMouseMove(float x, float y) {
	_mouse.x = x;
	_mouse.y = y;

	Element *newMouseOverElement = ElementAt((unsigned) x, (unsigned) y);

	if (newMouseOverElement != _mouseover_element) {
		if (_mouseover_element) {
			_mouseover_element->OnMouseExit(x, y);
		}

		_mouseover_element = newMouseOverElement;

		if (_mouseover_element) {
			_mouseover_element->OnMouseEnter(x, y);
		}
	}

	if (_mouseover_element) {
		_mouseover_element->OnMouseMove(x, y);
	}
}

void UI::OnMouseButton(Input::MouseButton button, Input::Action action, Input::Modifiers mods) {
	if (_mouseover_element) {
		switch (action) {
			case Input::Action::PRESS:
				if (_mouseover_element->IsFocusable()) {
					RequestFocus(_mouseover_element);
				}

				_mouseover_element->OnMouseButtonPress(button, mods);
				break;
			case Input::Action::REPEAT:
				break;
			case Input::Action::RELEASE:
				_mouseover_element->OnMouseButtonRelease(button, mods);
				break;
		}
	}
}

void UI::OnScroll(float x, float y) {
	if (_mouseover_element) {
		_mouseover_element->OnMouseScroll(x, y);
	} else if (_focus_element) {
		_focus_element->OnMouseScroll(x, y);
	}
}

void UI::OnFocusChanged(bool focus) {
	if (focus == _focus) {
		return;
	}

	_focus = focus;

	if (_focus_element) {
		if (_focus) {
			_focus_element->OnFocusGained();
		} else {
			_focus_element->OnFocusLost();
		}
	}
}

}
