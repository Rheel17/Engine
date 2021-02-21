/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_DUMMYWINDOW_H
#define RHEELENGINE_DUMMYWINDOW_H
#include "../../_common.h"

#include "Window.h"

namespace rheel {

class RE_API DummyWindow : public Window {

public:
	explicit DummyWindow(const Window& main_window);

	void CreateOglContext();

private:
	static window_hints _create_window_hints(const Window& main_window);

};

}

#endif
