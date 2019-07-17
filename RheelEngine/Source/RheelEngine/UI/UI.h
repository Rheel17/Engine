#ifndef UI_H_
#define UI_H_
#include "../_common.h"

#include "Elements/Container.h"

namespace rheel {

class RE_API UI {

public:
	/**
	 * Returns the container which contains the UI elements of this UI.
	 */
	std::shared_ptr<Container> GetContainer();

	/**
	 * Draws this UI
	 */
	void Draw() const;

	/**
	 * Creates a new empty UI.
	 */
	static std::shared_ptr<UI> Create();

private:
	UI() = default;

	std::shared_ptr<Container> _ui_container = Container::Create();

};

using UIPtr = std::shared_ptr<UI>;

}

#endif
