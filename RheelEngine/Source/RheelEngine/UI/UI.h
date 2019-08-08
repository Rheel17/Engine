#ifndef UI_H_
#define UI_H_
#include "../_common.h"

#include "Input.h"
#include "Elements/Container.h"

namespace rheel {

class RE_API UI {

public:
	/**
	 * Returns the container which contains the UI elements of this UI.
	 */
	Container *GetContainer();

	/**
	 * Draws this UI
	 */
	void Draw() const;

	/**
	 * Creates a new empty UI.
	 */
	static std::shared_ptr<UI> Create();

	void OnKey(Input::Key key, Input::Scancode scancode, Input::Action action, Input::Modifiers mods);

	void OnCharacter(wchar_t character);

	void OnMouseMove(float x, float y);

	void OnMouseButton(Input::MouseButton button, Input::Action action, Input::Modifiers mods);

	void OnScroll(float x, float y);

private:
	UI();

	Container *_ui_container;
	ElementPtr _focus_element;

};

using UIPtr = std::shared_ptr<UI>;

}

#endif
