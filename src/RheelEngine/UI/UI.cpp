/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "UI.h"

#include "../Engine.h"

namespace rheel {

UI::UI(unsigned width, unsigned height) :
		_width(width),
		_height(height) {}

void UI::SetContainer(Container&& container) {
	_ui_container->operator=(std::forward<Container>(container));
	_ui_container->_parent_ui = this;
	_ui_container->SetBounds({ 0, 0, _width, _height });
}

void UI::SetContainer(std::unique_ptr<Container> container) {
	_ui_container = std::move(container);
	_ui_container->_parent_ui = this;
	_ui_container->SetBounds({ 0, 0, _width, _height });
}

const Container& UI::GetContainer() const {
	return *_ui_container;
}

Element* UI::ElementAt(unsigned x, unsigned y) {
	return _ui_container->ElementAt(x, y);
}

bool UI::RequestFocus(Element* element) {
	if (_focus_element == element) {
		return true;
	}

	if (element && !element->IsFocusable()) {
		return false;
	}

	ReleaseMouse();

	if (_focus_element && _focus) {
		_focus_element->OnFocusLost_();
	}

	_focus_element = element;

	if (_focus_element && _focus) {
		_focus_element->OnFocusGained_();
	}

	return true;
}

Element* UI::FocusElement() const {
	return _focus_element;
}

void UI::GrabMouse(Element* element) {
	Engine::GetWindow().SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	_grabbed_element = element;
	_mouseover_element = element;
	_mouse_grabbed = true;
}

void UI::ReleaseMouse() {
	Engine::GetWindow().SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	_mouse = Engine::GetWindow().GetMousePosition();
	_grabbed_element = nullptr;
	_mouse_grabbed = false;
	_mouse_jump = true;
}

void UI::Draw(float time, float dt) const {
	_ui_container->Draw(time, dt);
}

void UI::OnKey(Input::Key key, Input::Scancode scancode, Input::Action action, Input::Modifiers mods) {
	if (_focus_element) {
		switch (action) {
			case Input::Action::PRESS:
				_focus_element->OnKeyPress_(key, scancode, mods);
				break;
			case Input::Action::REPEAT:
				_focus_element->OnKeyRepeat_(key, scancode, mods);
				break;
			case Input::Action::RELEASE:
				_focus_element->OnKeyRelease_(key, scancode, mods);
				break;
		}
	}
}

void UI::OnCharacter(Input::Unicode character) {
	if (_focus_element) {
		_focus_element->OnCharacterInput_(character);
	}
}

void UI::OnMouseMove(float x, float y) {
	_mouse.x = x;
	_mouse.y = y;

	if (_mouse_down_count > 0) {
		// a mouse button is down, so we drag
		_mouseover_element->OnMouseDrag_(_mouse_down_position, _mouse);
		return;
	}

	Element* newMouseOverElement = _ui_container->OpaqueElementAt((unsigned) x, (unsigned) y);

	if (newMouseOverElement != _mouseover_element && !_mouse_grabbed) {
		if (_mouseover_element) {
			_mouseover_element->OnMouseExit_(_mouse);
		}

		_mouseover_element = newMouseOverElement;

		if (_mouseover_element) {
			_mouseover_element->OnMouseEnter_(_mouse);
		}
	}

	if (_mouseover_element) {
		if (_mouse_jump) {
			_mouseover_element->OnMouseJump_(_mouse);
			_mouse_jump = false;
		} else {
			_mouseover_element->OnMouseMove_(_mouse);
		}
	}
}

void UI::OnMouseButton(Input::MouseButton button, Input::Action action, Input::Modifiers mods) {
	if (_mouseover_element) {
		switch (action) {
			case Input::Action::PRESS:
				if (++_mouse_down_count == 1) {
					_mouse_down_position = _mouse;
				}

				if (_mouseover_element->IsFocusable()) {
					RequestFocus(_mouseover_element);
				}

				_mouseover_element->OnMouseButtonPress_(button, mods);
				break;
			case Input::Action::REPEAT:
				break;
			case Input::Action::RELEASE:
				_mouse_down_count--;
				_mouseover_element->OnMouseButtonRelease_(button, mods);
				break;
		}
	}
}

void UI::OnScroll(float x, float y) {
	if (_mouseover_element) {
		_mouseover_element->OnMouseScroll_({ x, y });
	} else if (_focus_element) {
		_focus_element->OnMouseScroll_({ x, y });
	}
}

void UI::OnFocusChanged(bool focus) {
	if (focus == _focus) {
		return;
	}

	_focus = focus;

	if (_focus_element) {
		if (_focus) {
			_focus_element->OnFocusGained_();
		} else {
			_focus_element->OnFocusLost_();
		}
	}
}

}
