/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <sstream>

#include "../Engine.h"
#include "OpenGL/State.h"
#include "OpenGL/Debug.h"

namespace rheel {

bool Window::_is_glfw_initialized = false;

inline static GLFWwindow *getWindow(void *handle) {
	return static_cast<GLFWwindow *>(handle);
}

static void glfw_KeyCallback(GLFWwindow *glfw_window, int key, int scancode, int action, int mods);
static void glfw_CharCallback(GLFWwindow *glfw_window, unsigned int codepoint);
static void glfw_MouseMoveCallback(GLFWwindow *glfw_window, double xpos, double ypos);
static void glfw_MouseButtonCallback(GLFWwindow *glfw_window, int button, int action, int mods);
static void glfw_ScrollCallback(GLFWwindow *glfw_window, double x, double y);
static void glfw_WindowFocusCallback(GLFWwindow *glfw_window, int focus);

Window::Window(DisplayConfiguration& configuration) :
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef RE_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	// create the window and set the window handle for later user
	GLFWwindow *window = glfwCreateWindow(
			_configuration.resolution.width,
			_configuration.resolution.height,
			_configuration.title.c_str(),
			monitor, nullptr);

	_window_handle = window;

	// pass the actual window width and height back to the engine
	int realWidth, realHeight;
	glfwGetWindowSize(window, &realWidth, &realHeight);

	_configuration.resolution.width = realWidth;
	_configuration.resolution.height = realHeight;

	// initialize callbacks
	glfwSetKeyCallback(window, glfw_KeyCallback);
	glfwSetCharCallback(window, glfw_CharCallback);
	glfwSetCursorPosCallback(window, glfw_MouseMoveCallback);
	glfwSetMouseButtonCallback(window, glfw_MouseButtonCallback);
	glfwSetScrollCallback(window, glfw_ScrollCallback);
	glfwSetWindowFocusCallback(window, glfw_WindowFocusCallback);

	// initialize _OpenGL
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW.");
	}

	// FROM THIS POINT WE HAVE AN OPENGL CONTEXT!
	GL::Framebuffer::InitializeDefaultFramebuffer(uvec2{ unsigned(realWidth), unsigned(realHeight) });
	GL::State::Initialize();

	// enable or disable vsync
	if (!_configuration.vsync) {
		glfwSwapInterval(0);
	}

#ifdef RE_DEBUG
	// initialize OpenGL debugging
	GL::Debug::SetDebugCallback([](unsigned id, GL::Debug::Source source, GL::Debug::Type type,
			GL::Debug::Severity severity, const std::string& message) {

		if (id == 131169 || id == 131204 || id == 131218 || severity == GL::Debug::Severity::NOTIFICATION) {
			return;
		}

		std::stringstream ss;
		ss << "OpenGL " << id << " [" <<
				"source=" << GL::Debug::GetString(source) << ", " <<
				"type=" << GL::Debug::GetString(type) << "," <<
				"severity=" << GL::Debug::GetString(severity) << "]: " << message;

		switch (severity) {
			case GL::Debug::Severity::HIGH:         Log::Error() << ss.str() << std::endl; break;
			case GL::Debug::Severity::MEDIUM:       Log::Warning() << ss.str() << std::endl; break;
			case GL::Debug::Severity::LOW:
			case GL::Debug::Severity::NOTIFICATION: Log::Info() << ss.str() << std::endl; break;
		}
	});
#endif

	GL::State::SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GL::State::Enable(GL::Capability::DEPTH_TEST);
	GL::State::SetDepthFunction(GL::CompareFunction::LEQUAL);

	GL::State::Enable(GL::Capability::CULL_FACE);
	GL::State::SetCullFace(GL::CullFace::BACK);

	GL::State::Enable(GL::Capability::BLEND);
	GL::State::SetBlendFunction(GL::BlendFactor::SRC_ALPHA, GL::BlendFactor::ONE_MINUS_SRC_ALPHA);
}

void Window::Loop() {
	GLFWwindow *window = getWindow(_window_handle);

	double time = 0.0;

	while (!glfwWindowShouldClose(window)) {
		// handle all input events
		glfwPollEvents();

		// calculate the time delta
		float dt;
		double newTime = glfwGetTime();

		if (time == 0.0) {
			dt = 1.0f / 60.0f;
		} else {
			dt = static_cast<float>(newTime - time);
		}

		time = newTime;

		// update the scene
		if (auto scene = Engine::GetActiveScene(); scene) {
			scene->Update(time, dt);
			Engine::GetSceneRenderManager(scene).Update();
		}

		// initialize OpenGL state
		GL::State::ClearProgram();
		GL::Framebuffer::DefaultFramebuffer().Clear(GL::Framebuffer::BitField::COLOR_DEPTH);

		// draw the game
		Engine::GetUI().Draw(time, dt);

		// finish the update/render cycle
		glfwSwapBuffers(window);
		// TODO: GL state push/pop check for consistency
	}
}

void Window::SetInputMode(int mode, int value) const {
	glfwSetInputMode(getWindow(_window_handle), mode, value);
}

vec2 Window::GetMousePosition() const {
	double x, y;
	glfwGetCursorPos(getWindow(_window_handle), &x, &y);

	return vec2 { x, y };
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

static void glfw_KeyCallback(GLFWwindow *glfw_window, int key, int scancode, int action, int mods) {
	Engine::GetUI().OnKey(static_cast<Input::Key>(key), scancode, static_cast<Input::Action>(action), mods);
}

static void glfw_CharCallback(GLFWwindow *glfw_window, unsigned codepoint) {
	Engine::GetUI().OnCharacter(codepoint);
}

static void glfw_MouseMoveCallback(GLFWwindow *glfw_window, double xpos, double ypos) {
	Engine::GetUI().OnMouseMove(xpos, ypos);
}

static void glfw_MouseButtonCallback(GLFWwindow *glfw_window, int button, int action, int mods) {
	Engine::GetUI().OnMouseButton(static_cast<Input::MouseButton>(button), static_cast<Input::Action>(action), mods);
}

static void glfw_ScrollCallback(GLFWwindow *glfw_window, double x, double y) {
	Engine::GetUI().OnScroll(x, y);
}

static void glfw_WindowFocusCallback(GLFWwindow *glfw_window, int focus) {
	Engine::GetUI().OnFocusChanged(focus);
}

}
