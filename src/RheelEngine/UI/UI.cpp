/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "UI.h"

namespace rheel {

UI::UI(Game& game) :
		_game(game),
		_width(game.GetWindow().GetWindowSize().x),
		_height(game.GetWindow().GetWindowSize().y) {

	SetContainer(Container());
}

Game& UI::GetGame() {
	return _game;
}

void UI::SetContainer(Container&& container) {
	if (!_ui_container) {
		_ui_container = std::make_unique<Container>(std::move(container));
	} else {
		_ui_container->operator=(std::move(container));
		_init_container();
	}
}

void UI::SetContainer(std::unique_ptr<Container> container) {
	_ui_container = std::move(container);
	_init_container();
}

const Container& UI::GetContainer() const {
	return *_ui_container;
}

Element* UI::GetElementAt(unsigned x, unsigned y) {
	return _ui_container->ElementAt(x, y);
}

bool UI::RequestFocus(Element* element) {
	if (_focus_element == element) {
		return true;
	}

	if (element && !element->IsFocusable()) {
		Log::Warning() << "Requested focus on non-focusable element" << std::endl;
		return false;
	}

	ReleaseMouse();

	if (_focus_element && _focus) {
		_focus_element->_on_focus_lost();
	}

	_focus_element = element;

	if (_focus_element && _focus) {
		_focus_element->_on_focus_gained();
	}

	return true;
}

Element* UI::GetFocusElement() const {
	return _focus_element;
}

void UI::GrabMouse(Element* element) {
	_game.GetWindow().SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	_mouseover_element = element;
	_mouse_grabbed = true;
}

void UI::ReleaseMouse() {
	_game.GetWindow().SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	_mouse = _game.GetWindow().GetMousePosition();
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
				_focus_element->_on_key_press(key, scancode, mods);
				break;
			case Input::Action::REPEAT:
				_focus_element->_on_key_repeat(key, scancode, mods);
				break;
			case Input::Action::RELEASE:
				_focus_element->_on_key_release(key, scancode, mods);
				break;
		}
	}
}

void UI::OnCharacter(Input::Unicode character) {
	if (_focus_element) {
		_focus_element->_on_character_input(character);
	}
}

void UI::OnMouseMove(float x, float y) {
	_mouse.x = x;
	_mouse.y = y;

	if (_mouse_down_count > 0 && _mouseover_element != nullptr && _mouseover_element->IsDragEnabled()) {
		// a mouse button is down, so we drag
		_mouseover_element->_on_mouse_drag(_mouse_down_position, _mouse);
		return;
	}

	Element* new_mouse_over_element = _ui_container->OpaqueElementAt((unsigned) x, (unsigned) y);

	if (new_mouse_over_element != _mouseover_element && !_mouse_grabbed) {
		if (_mouseover_element) {
			_mouseover_element->_on_mouse_exit(_mouse);
		}

		_mouseover_element = new_mouse_over_element;

		if (_mouseover_element) {
			_mouseover_element->_on_mouse_enter(_mouse);
		}
	}

	if (_clicked_element != nullptr && _clicked_element != _mouseover_element) {
		// we moved the mouse away from the element we clicked, so cancel the
		// click
		_clicked_element = nullptr;
	}

	if (_mouseover_element) {
		if (_mouse_jump) {
			_mouseover_element->_on_mouse_jump(_mouse);
			_mouse_jump = false;
		} else {
			_mouseover_element->_on_mouse_move(_mouse);
		}
	}
}

void UI::OnMouseButton(Input::MouseButton button, Input::Action action, Input::Modifiers mods) {
	if (_mouseover_element) {
		switch (action) {
			case Input::Action::PRESS:
				_clicked_element = _mouseover_element;

				if (++_mouse_down_count == 1) {
					_mouse_down_position = _mouse;
				}

				if (_clicked_element->IsFocusable()) {
					RequestFocus(_clicked_element);
				}

				_clicked_element->_on_mouse_button_press(button, mods);
				break;
			case Input::Action::REPEAT:
				break;
			case Input::Action::RELEASE:
				if (_clicked_element) {
					_mouse_down_count--;
					_clicked_element->_on_mouse_button_release(button, mods);
				}
				break;
		}
	}
}

void UI::OnScroll(float x, float y) {
	if (_mouseover_element) {
		_mouseover_element->_on_mouse_scroll({ x, y });
	} else if (_focus_element) {
		_focus_element->_on_mouse_scroll({ x, y });
	}
}

void UI::OnFocusChanged(bool focus) {
	if (focus == _focus) {
		return;
	}

	_focus = focus;

	if (_focus_element) {
		if (_focus) {
			_focus_element->_on_focus_gained();
		} else {
			_focus_element->_on_focus_lost();
		}
	}
}

void UI::_init_container() {
	_ui_container->_parent_ui = this;
	_ui_container->SetBounds({ 0, 0, _width, _height });

	ReleaseMouse();
	_mouseover_element = nullptr;
	_clicked_element = nullptr;
	_focus_element = nullptr;

	if (_ui_container->IsFocusable()) {
		RequestFocus(_ui_container.get());
	}
}

}
