#ifndef UI_H_
#define UI_H_
#include "../_common.h"

#include "Container.h"

namespace rheel {

class RE_API UI {

	RE_NO_COPY(UI);
	RE_NO_MOVE(UI);

public:
	UI(unsigned width, unsigned height);

	/**
	 * Sets the contents of the UI to the container.
	 */
	void SetContainer(Container&& container);

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
	Element *ElementAt(unsigned x, unsigned y);

	/**
	 * Requests that the given element be given focus in this UI.
	 */
	void RequestFocus(Element *element);

	/**
	 * Returns the element which is in focus (can be nullptr).
	 */
	Element *FocusElement() const;

	/**
	 * Grabs the mouse such that it is no longer capable of leaving the window.
	 * It will also be made invisible. Use Input::ReleaseMouse() to undo this
	 * action.
	 *
	 * A grabbed mouse will be restricted the given element. On a focus change,
	 * the mouse is automatically released.
	 */
	void GrabMouse(Element *element);

	/**
	 * Releases the mouse such that it is visible and capable of leaving the
	 * window again. Use Input::GrabMouse() to grab the mouse again.
	 */
	void ReleaseMouse();

	/**
	 * Draws this UI
	 */
	void Draw(float dt) const;

	void OnKey(Input::Key key, Input::Scancode scancode, Input::Action action, Input::Modifiers mods);

	void OnCharacter(wchar_t character);

	void OnMouseMove(float x, float y);

	void OnMouseButton(Input::MouseButton button, Input::Action action, Input::Modifiers mods);

	void OnScroll(float x, float y);

	void OnFocusChanged(bool focus);

private:
	Container _ui_container = Container(this);

	Element *_mouseover_element = nullptr;
	Element *_focus_element = nullptr;
	Element *_grabbed_element = nullptr;

	unsigned _width;
	unsigned _height;

	bool _focus = true;
	bool _mouse_grabbed = false;
	vec2 _mouse;

	unsigned _mouse_down_count = 0;
	vec2 _mouse_down_position;

};

using UIPtr = std::shared_ptr<UI>;

}

#endif
