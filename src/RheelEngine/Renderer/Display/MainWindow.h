/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_MAINWINDOW_H
#define RHEELENGINE_MAINWINDOW_H
#include "../../_common.h"

#include "DisplayConfiguration.h"
#include "Window.h"

namespace rheel {

class RE_API MainWindow : public Window {
	RE_NO_MOVE(MainWindow);
	RE_NO_COPY(MainWindow);

public:
	/**
	 * Creates a window using the given display configuration.
	 * Use Show() to show the window.
	 */
	explicit MainWindow(DisplayConfiguration& configuration);
	~MainWindow();

	void Loop();

	void SetInputMode(int mode, int value) const;
	vec2 GetMousePosition() const;

private:
	static window_hints CreateWindowHints_(const DisplayConfiguration& configuration);

};

}

#endif
