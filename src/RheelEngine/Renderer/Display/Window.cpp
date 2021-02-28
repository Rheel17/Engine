/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Window.h"

namespace rheel {

void window_hints::Set() const {
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_VISIBLE, visible);
	glfwWindowHint(GLFW_DOUBLEBUFFER, doublebuffer);
	glfwWindowHint(GLFW_DECORATED, decorated);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_version_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, opengl_profile);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, opengl_debug_context);
}

Window::Window(window_hints hints, unsigned width, unsigned height, const std::string& title, GLFWmonitor* monitor) :
		_hints(hints) {

	// Apply the window hints
	hints.Set();

	// Create the window
	handle = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);

	if (handle == nullptr) {
		Log::Error() << "Unable to create window" << std::endl;
		abort();
	}

	glfwSetWindowUserPointer(handle, this);
}

Window::Window(window_hints hints, unsigned int width, unsigned int height, const std::string& title, GLFWmonitor* monitor, const Window& share) :
		_hints(hints) {

	// Apply the window hints
	hints.Set();

	// Create the window
	handle = glfwCreateWindow(width, height, title.c_str(), monitor, share.handle);

	if (handle == nullptr) {
		Log::Error() << "Unable to create window" << std::endl;
		abort();
	}

	glfwSetWindowUserPointer(handle, this);
}

Window::~Window() {
	if (handle != nullptr) {
		glfwMakeContextCurrent(nullptr);
		glfwDestroyWindow(handle);
	}
}

Window::Window(Window&& window) noexcept:
		handle(window.handle),
		_hints(std::move(window._hints)) {

	window.handle = nullptr;

	glfwSetWindowUserPointer(handle, this);
}

const window_hints& Window::GetWindowHints() const {
	return _hints;
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(handle);
}

void Window::Close() const {
	glfwSetWindowShouldClose(handle, true);
}

uvec2 Window::GetWindowSize() const {
	int width, height;
	glfwGetWindowSize(handle, &width, &height);
	return uvec2{ width, height };
}

gl::Context& Window::GetContext() {
	return *_context;
}

const gl::Context& Window::GetContext() const {
	return *_context;
}

void Window::SetVisible(bool visible) {
	if (visible) {
		glfwShowWindow(handle);
	} else {
		glfwHideWindow(handle);
	}
}

void Window::CreateContext() {
	glfwMakeContextCurrent(handle);

	if (glewInit() != GLEW_OK) {
		Log::Error() << "Could not initialize GLEW" << std::endl;
		abort();
	}

	_context = std::unique_ptr<gl::Context>(new gl::Context(GetWindowSize()));
}

GLFWmonitor* Window::GetPrimaryMonitor() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	return monitor;
}

void Window::RunAfterCurrentFrame(std::function<void()> f) {
	after_frame_queue.push_back(std::move(f));
}

}
