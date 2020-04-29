/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "MainWindow.h"

#include <sstream>

#include "../../Engine.h"
#include "../OpenGL/Debug.h"

namespace rheel {

static void glfwKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
static void glfwCharCallback(GLFWwindow* glfwWindow, unsigned int codepoint);
static void glfwMouseMoveCallback(GLFWwindow* glfwWindow, double xpos, double ypos);
static void glfwMouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
static void glfwScrollCallback(GLFWwindow* glfwWindow, double x, double y);
static void glfwWindowFocusCallback(GLFWwindow* glfwWindow, int focus);

MainWindow::MainWindow(DisplayConfiguration& configuration) :
		Window(CreateWindowHints_(configuration),
				configuration.resolution.x,
				configuration.resolution.y,
				configuration.title,
				configuration.window_mode == DisplayConfiguration::WindowMode::FULLSCREEN ? GetPrimaryMonitor() : nullptr) {

	// pass the actual window width and height back to the engine
	auto size = GetWindowSize();
	configuration.resolution.x = size.x;
	configuration.resolution.y = size.y;

	// initialize callbacks
	glfwSetKeyCallback(handle, glfwKeyCallback);
	glfwSetCharCallback(handle, glfwCharCallback);
	glfwSetCursorPosCallback(handle, glfwMouseMoveCallback);
	glfwSetMouseButtonCallback(handle, glfwMouseButtonCallback);
	glfwSetScrollCallback(handle, glfwScrollCallback);
	glfwSetWindowFocusCallback(handle, glfwWindowFocusCallback);

	// create the OpenGL context, and make it current
	CreateContext();

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

	// initialize OpenGL state
	GetContext().SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GetContext().Enable(gl::Capability::DEPTH_TEST);
	GetContext().SetDepthFunction(gl::CompareFunction::LEQUAL);

	GetContext().Enable(gl::Capability::CULL_FACE);
	GetContext().SetCullFace(gl::CullFace::BACK);

	GetContext().Enable(gl::Capability::BLEND);
	GetContext().SetBlendFunction(gl::BlendFactor::SRC_ALPHA, gl::BlendFactor::ONE_MINUS_SRC_ALPHA);
}

MainWindow::~MainWindow() {
	Log::Info() << "Closing Window" << std::endl;
}

void MainWindow::Loop() {
	double time = 0.0;

	while (!ShouldClose()) {
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

		// run UI thread tasks
		{
			std::lock_guard lock(_mutex);
			while (!_task_queue.empty()) {
				auto task = std::move(_task_queue.front());
				_task_queue.pop();
				task->operator()();
			}
		}

		// update the scene
		if (auto scene = Engine::GetActiveScene(); scene) {
			scene->Update(time, dt);
			Engine::GetSceneRenderManager(scene).Update();
		}

		// initialize OpenGL state
		gl::Context::Current().ClearProgram();
		gl::Framebuffer::DefaultFramebuffer().Clear(gl::Framebuffer::BitField::COLOR_DEPTH);

		// draw the game
		Engine::GetUI().Draw(time, dt);

		// finish the update/render cycle
		if (GetWindowHints().doublebuffer) {
			glfwSwapBuffers(handle);
		} else {
			glFlush();
		}

		gl::Context::Current().CheckStackConsistency();
	}
}

void MainWindow::SetInputMode(int mode, int value) const {
	glfwSetInputMode(handle, mode, value);
}

vec2 MainWindow::GetMousePosition() const {
	double x, y;
	glfwGetCursorPos(handle, &x, &y);

	return vec2{ x, y };
}

window_hints MainWindow::CreateWindowHints_(const DisplayConfiguration& configuration) {
	window_hints hints;
	hints.resizable = false;
	hints.doublebuffer = configuration.vsync;

#ifdef RE_DEBUG
	hints.opengl_debug_context = true;
#endif

	return hints;
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
