#ifndef WINDOW_H_
#define WINDOW_H_
#include "../_common.h"

#include <memory>

#include "DisplayConfiguration.h"

namespace rheel {

class RE_API Window {

public:
	/**
	 * Creates a window using the given display configuration.
	 * Use Show() to show the window.
	 */
	Window(DisplayConfiguration& configuration);
	~Window();

	void Show();
	void Loop();

	void SetInputMode(int mode, int value) const;

private:
	DisplayConfiguration& _configuration;

	void *_window_handle = nullptr;

public:
	static void InitializeDisplaySystems();
	static void DestroyDisplaySystems();

private:
	static bool _is_glfw_initialized;

};

}

#endif
