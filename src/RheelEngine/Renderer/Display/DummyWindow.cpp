/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "DummyWindow.h"

namespace rheel {

DummyWindow::DummyWindow(const MainWindow& mainWindow) :
		Window(CreateWindowHints_(mainWindow), 1, 1, "", nullptr, mainWindow) {
}

window_hints DummyWindow::CreateWindowHints_(const MainWindow& mainWindow) {
	window_hints hints;
	hints.visible = false;
	hints.context_version_major = mainWindow.GetWindowHints().context_version_major;
	hints.context_version_minor = mainWindow.GetWindowHints().context_version_minor;
	hints.opengl_profile = mainWindow.GetWindowHints().opengl_profile;
	return hints;
}

void DummyWindow::CreateOglContext() {
	CreateContext();
}

}