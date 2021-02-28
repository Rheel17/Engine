/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "MainWindow.h"

#include <sstream>

#include "../../Game.h"
#include "../../Scene.h"
#include "../../UI/UI.h"
#include "../OpenGL/Debug.h"

namespace rheel {

void glfw_key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods); // NOLINT (false positive)
void glfw_char_callback(GLFWwindow* glfw_window, unsigned int codepoint);                     // NOLINT (false positive)
void glfw_mouse_move_callback(GLFWwindow* glfw_window, double xpos, double ypos);             // NOLINT (false positive)
void glfw_mouse_button_callback(GLFWwindow* glfw_window, int button, int action, int mods);   // NOLINT (false positive)
void glfw_scroll_callback(GLFWwindow* glfw_window, double x, double y);                       // NOLINT (false positive)
void glfw_window_focus_callback(GLFWwindow* glfw_window, int focus);                          // NOLINT (false positive)

MainWindow::MainWindow(DisplayConfiguration& configuration, const std::string& title, Game& game) :
		Window(_create_window_hints(configuration),
				configuration.resolution.x,
				configuration.resolution.y,
				title,
				configuration.window_mode == DisplayConfiguration::WindowMode::FULLSCREEN ? GetPrimaryMonitor() : nullptr),
		_game(game) {

	// pass the actual window width and height back to the engine
	auto size = GetWindowSize();
	configuration.resolution.x = size.x;
	configuration.resolution.y = size.y;

	// initialize callbacks
	glfwSetKeyCallback(handle, glfw_key_callback);
	glfwSetCharCallback(handle, glfw_char_callback);
	glfwSetCursorPosCallback(handle, glfw_mouse_move_callback);
	glfwSetMouseButtonCallback(handle, glfw_mouse_button_callback);
	glfwSetScrollCallback(handle, glfw_scroll_callback);
	glfwSetWindowFocusCallback(handle, glfw_window_focus_callback);

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

	GetContext().Enable(gl::Capability::CULL_FACE);
	GetContext().SetCullFace(gl::CullFace::BACK);

	GetContext().Enable(gl::Capability::BLEND);
	GetContext().SetBlendFunction(gl::BlendFactor::SRC_ALPHA, gl::BlendFactor::ONE_MINUS_SRC_ALPHA);
}

MainWindow::~MainWindow() {
	Log::Info() << "Closing Window" << std::endl;
}

void MainWindow::Loop() {
	float time = 0.0f;

	while (!ShouldClose()) {
		// handle all input events
		glfwPollEvents();

		// calculate the time delta
		float dt;
		auto new_time = static_cast<float>(glfwGetTime());

		if (time == 0.0) {
			dt = 1.0f / 60.0f;
		} else {
			dt = new_time - time;
		}

		time = new_time;

		// run UI thread tasks
		{
			std::lock_guard lock(_mutex);
			while (!_task_queue.empty()) {
				auto task = std::move(_task_queue.front());
				_task_queue.pop();

				GetContext().Push();
				task->operator()();
				GetContext().Pop();
			}
		}

		// update the scene
		if (auto scene = _game.GetActiveScene(); scene) {
			scene->Update(time, dt);
			_game.GetRenderer().GetSceneRenderManager(scene).Update();
		}

		// initialize OpenGL state
		GetContext().Push();
		gl::Framebuffer::DefaultFramebuffer().Clear(gl::Framebuffer::BitField::COLOR_DEPTH);

		// draw the game
		_game.GetUI().Draw(time, dt);

		// perform the after-frame queue
		for (const auto& f : after_frame_queue) {
			f();
		}

		after_frame_queue.clear();

		// finish the update/render cycle
		GetContext().Pop();

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

window_hints MainWindow::_create_window_hints(const DisplayConfiguration& configuration) {
	window_hints hints;
	hints.visible = false;
	hints.resizable = false;
	hints.doublebuffer = configuration.vsync;
	hints.decorated = configuration.window_mode != DisplayConfiguration::BORDERLESS_WINDOW;

#ifdef RE_DEBUG
	hints.opengl_debug_context = true;
#endif

	return hints;
}

void glfw_key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods) {
	auto window = static_cast<MainWindow*>(glfwGetWindowUserPointer(glfw_window));
	window->_game.GetUI().OnKey(static_cast<Input::Key>(key), scancode, static_cast<Input::Action>(action), mods);
}

void glfw_char_callback(GLFWwindow* glfw_window, unsigned codepoint) {
	auto window = static_cast<MainWindow*>(glfwGetWindowUserPointer(glfw_window));
	window->_game.GetUI().OnCharacter(codepoint);
}

void glfw_mouse_move_callback(GLFWwindow* glfw_window, double xpos, double ypos) {
	auto window = static_cast<MainWindow*>(glfwGetWindowUserPointer(glfw_window));
	window->_game.GetUI().OnMouseMove(static_cast<float>(xpos), static_cast<float>(ypos));
}

void glfw_mouse_button_callback(GLFWwindow* glfw_window, int button, int action, int mods) {
	auto window = static_cast<MainWindow*>(glfwGetWindowUserPointer(glfw_window));
	window->_game.GetUI().OnMouseButton(static_cast<Input::MouseButton>(button), static_cast<Input::Action>(action), mods);
}

void glfw_scroll_callback(GLFWwindow* glfw_window, double x, double y) {
	auto window = static_cast<MainWindow*>(glfwGetWindowUserPointer(glfw_window));
	window->_game.GetUI().OnScroll(x, y);
}

void glfw_window_focus_callback(GLFWwindow* glfw_window, int focus) {
	auto window = static_cast<MainWindow*>(glfwGetWindowUserPointer(glfw_window));
	window->_game.GetUI().OnFocusChanged(focus);
}

}
