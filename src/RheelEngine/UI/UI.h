/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_UI_H
#define RHEELENGINE_UI_H
#include "../_common.h"

#include "Container.h"
#include "../Game.h"
#include "../Renderer/Display/MainWindow.h"

namespace rheel {

class RE_API UI {
	RE_NO_COPY(UI);
	RE_NO_MOVE(UI);

public:
	/**
	 * Constructs the UI object. This instance will hold a reference to the
	 * passed-trough game reference.
	 */
	explicit UI(Game& game);

	/**
	 * Returns the corresponding game of this UI.
	 */
	Game& GetGame();

	/**
	 * Sets the contents of the UI to the container. Note: for custom
	 * subclasses of Container use the
	 * SetContainer(std::unqiue_ptr<Container>) overload to prevent
	 * object slicing.
	 */
	void SetContainer(Container&& container);

	/**
	 * Sets the contents of the UI to the container. Use this overload for
	 * custom subclasses of Container.
	 */
	void SetContainer(std::unique_ptr<Container> container);

	/**
	 * Returns the container which contains the UI elements of this UI.
	 */
	const Container& GetContainer() const;

	/**
	 * Returns the element at the specified position. When no element was added
	 * at the specified position, the root container of this UI is returned. If
	 * multiple elements share the position, the top one (last added) is
	 * returned.
	 */
	Element* GetElementAt(unsigned x, unsigned y);

	/**
	 * Requests that the given element be given focus in this UI.
	 */
	bool RequestFocus(Element* element);

	/**
	 * Returns the element which is in focus (can be nullptr).
	 */
	Element* GetFocusElement() const;

	/**
	 * Grabs the mouse such that it is no longer capable of leaving the window.
	 * It will also be made invisible. Use Input::ReleaseMouse() to undo this
	 * action.
	 *
	 * A grabbed mouse will be restricted the given element. On a focus change,
	 * the mouse is automatically released.
	 */
	void GrabMouse(Element* element);

	/**
	 * Releases the mouse such that it is visible and capable of leaving the
	 * window again. Use Input::GrabMouse() to grab the mouse again.
	 */
	void ReleaseMouse();

	/**
	 * Draws this UI
	 */
	void Draw(float time, float dt) const;

	void OnKey(Input::Key key, Input::Scancode scancode, Input::Action action, Input::Modifiers mods);

	void OnCharacter(Input::Unicode character);

	void OnMouseMove(float x, float y);

	void OnMouseButton(Input::MouseButton button, Input::Action action, Input::Modifiers mods);

	void OnScroll(float x, float y);

	void OnFocusChanged(bool focus);

private:
	void InitContainer_();

	std::unique_ptr<Container> _ui_container;

	Game& _game;

	Element* _mouseover_element = nullptr;
	Element* _focus_element = nullptr;
	Element* _grabbed_element = nullptr;

	unsigned _width;
	unsigned _height;

	bool _focus = true;
	bool _mouse_grabbed = false;
	bool _mouse_jump = false;
	vec2 _mouse;

	unsigned _mouse_down_count = 0;
	vec2 _mouse_down_position;

};

}

#endif
