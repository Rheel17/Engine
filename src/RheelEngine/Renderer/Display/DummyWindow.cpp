/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "DummyWindow.h"

namespace rheel {

DummyWindow::DummyWindow(const Window& main_window) :
		Window(_create_window_hints(main_window), 1, 1, "", nullptr, main_window) {
}

window_hints DummyWindow::_create_window_hints(const Window& main_window) {
	window_hints hints;
	hints.visible = false;
	hints.context_version_major = main_window.GetWindowHints().context_version_major;
	hints.context_version_minor = main_window.GetWindowHints().context_version_minor;
	hints.opengl_profile = main_window.GetWindowHints().opengl_profile;
	return hints;
}

void DummyWindow::CreateOglContext() {
	CreateContext();
}

}