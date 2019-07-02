#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Engine.h"
#include "../PerspectiveCamera.h"

namespace rheel {

bool Window::_is_glfw_initialized = false;

inline static GLFWwindow *getWindow(void *handle) {
	return static_cast<GLFWwindow *>(handle);
}

Window::Window(DisplayConfiguration configuration) :
		_configuration(configuration) {}

Window::~Window() {
	if (_window_handle) {
		GLFWwindow *window = getWindow(_window_handle);
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}

void Window::Show() {
	// apply the window parameters
	glfwDefaultWindowHints();
	GLFWmonitor *monitor = nullptr;

	switch (_configuration.window_mode) {
		case DisplayConfiguration::WindowMode::FULLSCREEN:
			glfwWindowHint(GLFW_RESIZABLE, false);
			monitor = glfwGetPrimaryMonitor();
			break;
		case DisplayConfiguration::WindowMode::WINDOWED_UNRESIZABLE:
			glfwWindowHint(GLFW_RESIZABLE, false);
			break;
		default:
			throw std::runtime_error("window mode not supported by OpenGLWindow.");
	}

	glfwWindowHint(GLFW_SAMPLES, _configuration.SampleCount());

	// create the window and set the window handle for later user
	GLFWwindow *window = glfwCreateWindow(
			_configuration.resolution.width,
			_configuration.resolution.height,
			_configuration.title.c_str(),
			monitor, nullptr);

	_window_handle = window;

	// enable or disable vsync
	if (!_configuration.vsync) {
		glfwSwapInterval(0);
	}

	// initialize OpenGL
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW.");
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Window::Loop() {
	GLFWwindow *window = getWindow(_window_handle);

	// run the window
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Engine::UpdateScenes();

		if (auto ui = Engine::GetUI()) {
			ui->Draw();
		}

		glfwSwapBuffers(window);
	}
}

void Window::InitializeDisplaySystems() {
	// initialize GLFW
	if (!_is_glfw_initialized) {
		if (!glfwInit()) {
			throw std::runtime_error("Failed to initialize GLFW.");
		}

		_is_glfw_initialized = true;
	}
}

void Window::DestroyDisplaySystems() {
	// terminate GLFW
	if (_is_glfw_initialized) {
		glfwTerminate();
	}
}

}
