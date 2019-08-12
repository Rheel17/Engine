#ifndef UI_H_
#define UI_H_
#include "../_common.h"

#include "Container.h"

namespace rheel {

class RE_API UI {

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
	 * Draws this UI
	 */
	void Draw() const;

	void OnKey(Input::Key key, Input::Scancode scancode, Input::Action action, Input::Modifiers mods);

	void OnCharacter(wchar_t character);

	void OnMouseMove(float x, float y);

	void OnMouseButton(Input::MouseButton button, Input::Action action, Input::Modifiers mods);

	void OnScroll(float x, float y);

private:
	Container _ui_container;
	ElementPtr _focus_element;

	unsigned _width;
	unsigned _height;

	vec2 _mouse;

};

using UIPtr = std::shared_ptr<UI>;

}

#endif
