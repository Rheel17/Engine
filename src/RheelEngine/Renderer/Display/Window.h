/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_WINDOW_H
#define RHEELENGINE_WINDOW_H
#include "../../_common.h"

// Don't include OpenGL, we have GLEW for that
#ifdef GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#else
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE
#endif

#include "../OpenGL/Context.h"

namespace rheel {

struct window_hints {
	enum Profile {
		OPENGL_ANY_PROFILE = GLFW_OPENGL_ANY_PROFILE,
		OPENGL_COMPAT_PROFILE = GLFW_OPENGL_COMPAT_PROFILE,
		OPENGL_CORE_PROFILE = GLFW_OPENGL_CORE_PROFILE
	};

	bool resizable = true;
	bool visible = true;
	bool doublebuffer = true;

	uint8_t context_version_major = 4;
	uint8_t context_version_minor = 3;
	Profile opengl_profile = OPENGL_CORE_PROFILE;
	bool opengl_debug_context = false;

	void Set() const;
};

class RE_API Window {
	RE_NO_COPY(Window);

public:
	/**
	 * Creates a window. Most parameters are hints, use asociated getters to
	 * retrieve the actual values.
	 */
	Window(window_hints hints, unsigned width, unsigned height, const std::string& title, GLFWmonitor* monitor = nullptr);
	Window(window_hints hints, unsigned width, unsigned height, const std::string& title, GLFWmonitor* monitor, const Window& share);
	virtual ~Window();

	Window(Window&& window) noexcept;
	Window& operator=(Window&& window) noexcept = delete;

	/**
	 * Returns the window hints this window was created with.
	 */
	const window_hints& GetWindowHints() const;

	/**
	 * Returns whether a close request has been sent to the window.
	 */
	bool ShouldClose() const;

	/**
	 * Sets ShouldClose to true.
	 */
	void Close() const;

	/**
	 * Returns the actual size of the window.
	 */
	uvec2 GetWindowSize() const;

	/**
	 * Returns the OpenGL context for this window, if is exists.
	 */
	gl::Context& GetContext();

	/**
	 * Returns the OpenGL context for this window, if is exists.
	 */
	const gl::Context& GetContext() const;

	/**
	 * If visible is true, this shows the window. If visible is false, this
	 * hides the window
	 */
	void SetVisible(bool visible);

protected:
	/**
	 * Creates an OpenGL context for this window, and makes it the context on
	 * the current thread. This method must be used in the thread that will use
	 * the context.
	 *
	 * There is currently no support for context switching between threads.
	 */
	void CreateContext();

	GLFWwindow* handle;

private:
	window_hints _hints;
	std::unique_ptr<gl::Context> _context;

public:
	static GLFWmonitor* GetPrimaryMonitor();

};

}

#endif
