#ifndef UI_H_
#define UI_H_
#include "../_common.h"

#include "Input.h"
#include "Container.h"

namespace rheel {

class RE_API UI {

public:
	UI(unsigned width, unsigned height);

	/**
	 * Sets the contents of the UI to the container.
	 */
	void SetContainer(const Container& container);

	/**
	 * Returns the container which contains the UI elements of this UI.
	 */
	const Container& GetContainer() const;

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

};

using UIPtr = std::shared_ptr<UI>;

}

#endif
