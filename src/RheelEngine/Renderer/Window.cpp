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

inline static GLFWwindow* getWindow(void* handle) {
	return static_cast<GLFWwindow*>(handle);
}

static void glfwKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
static void glfwCharCallback(GLFWwindow* glfwWindow, unsigned int codepoint);
static void glfwMouseMoveCallback(GLFWwindow* glfwWindow, double xpos, double ypos);
static void glfwMouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
static void glfwScrollCallback(GLFWwindow* glfwWindow, double x, double y);
static void glfwWindowFocusCallback(GLFWwindow* glfwWindow, int focus);

Window::Window(DisplayConfiguration& configuration) :
		_configuration(configuration) {}

Window::~Window() {
	if (_window_handle) {
		Log::Info() << "Closing Window" << std::endl;

		GLFWwindow* window = getWindow(_window_handle);
		glfwDestroyWindow(window);
	}
}

void Window::Show() {
	// apply the window parameters
	glfwDefaultWindowHints();
	GLFWmonitor* monitor = nullptr;

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
	GLFWwindow* window = glfwCreateWindow(
			_configuration.resolution.x,
			_configuration.resolution.y,
			_configuration.title.c_str(),
			monitor, nullptr);

	_window_handle = window;

	// pass the actual window width and height back to the engine
	int realWidth, realHeight;
	glfwGetWindowSize(window, &realWidth, &realHeight);

	_configuration.resolution.x = realWidth;
	_configuration.resolution.y = realHeight;

	// initialize callbacks
	glfwSetKeyCallback(window, glfwKeyCallback);
	glfwSetCharCallback(window, glfwCharCallback);
	glfwSetCursorPosCallback(window, glfwMouseMoveCallback);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
	glfwSetScrollCallback(window, glfwScrollCallback);
	glfwSetWindowFocusCallback(window, glfwWindowFocusCallback);

	// initialize OpenGL
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW.");
	}

	// FROM THIS POINT WE HAVE AN OPENGL CONTEXT!
	gl::Framebuffer::InitializeDefaultFramebuffer(uvec2{ unsigned(realWidth), unsigned(realHeight) });
	gl::State::Initialize();

	// enable or disable vsync
	if (!_configuration.vsync) {
		glfwSwapInterval(0);
	}

#ifdef RE_DEBUG
	// initialize OpenGL debugging
	gl::Debug::SetDebugCallback([](unsigned id, gl::Debug::Source source, gl::Debug::Type type, gl::Debug::Severity severity, const std::string& message) {
		if (id == 131169 || id == 131204 || id == 131218 || severity == gl::Debug::Severity::NOTIFICATION) {
			return;
		}

		std::stringstream ss;
		ss << "OpenGL " << id << " [" <<
				"source=" << gl::Debug::GetString(source) << ", " <<
				"type=" << gl::Debug::GetString(type) << ", " <<
				"severity=" << gl::Debug::GetString(severity) << "]: " << message;

		switch (severity) {
			case gl::Debug::Severity::HIGH:
				Log::Error() << ss.str() << std::endl;
				break;
			case gl::Debug::Severity::MEDIUM:
				Log::Warning() << ss.str() << std::endl;
				break;
			case gl::Debug::Severity::LOW:
			case gl::Debug::Severity::NOTIFICATION:
				Log::Info() << ss.str() << std::endl;
				break;
		}
	});
#endif

	gl::State::SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	gl::State::Enable(gl::Capability::DEPTH_TEST);
	gl::State::SetDepthFunction(gl::CompareFunction::LEQUAL);

	gl::State::Enable(gl::Capability::CULL_FACE);
	gl::State::SetCullFace(gl::CullFace::BACK);

	gl::State::Enable(gl::Capability::BLEND);
	gl::State::SetBlendFunction(gl::BlendFactor::SRC_ALPHA, gl::BlendFactor::ONE_MINUS_SRC_ALPHA);
}

void Window::Loop() {
	GLFWwindow* window = getWindow(_window_handle);

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
		gl::State::ClearProgram();
		gl::Framebuffer::DefaultFramebuffer().Clear(gl::Framebuffer::BitField::COLOR_DEPTH);

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

	return vec2{ x, y };
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

static void glfwKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
	Engine::GetUI().OnKey(static_cast<Input::Key>(key), scancode, static_cast<Input::Action>(action), mods);
}

static void glfwCharCallback(GLFWwindow* glfwWindow, unsigned codepoint) {
	Engine::GetUI().OnCharacter(codepoint);
}

static void glfwMouseMoveCallback(GLFWwindow* glfwWindow, double xpos, double ypos) {
	Engine::GetUI().OnMouseMove(xpos, ypos);
}

static void glfwMouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods) {
	Engine::GetUI().OnMouseButton(static_cast<Input::MouseButton>(button), static_cast<Input::Action>(action), mods);
}

static void glfwScrollCallback(GLFWwindow* glfwWindow, double x, double y) {
	Engine::GetUI().OnScroll(x, y);
}

static void glfwWindowFocusCallback(GLFWwindow* glfwWindow, int focus) {
	Engine::GetUI().OnFocusChanged(focus);
}

}
