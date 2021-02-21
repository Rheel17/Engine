/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_MAINWINDOW_H
#define RHEELENGINE_MAINWINDOW_H
#include "../../_common.h"

#include <queue>

#include "DisplayConfiguration.h"
#include "Window.h"
#include "../../Task.h"

namespace rheel {

class Game;

class RE_API MainWindow : public Window {
	RE_NO_MOVE(MainWindow);
	RE_NO_COPY(MainWindow);

public:
	/**
	 * Creates a window using the given display configuration and title. A
	 * reference to the game will be kept.
	 */
	MainWindow(DisplayConfiguration& configuration, const std::string& title, Game& game);
	~MainWindow() override;

	void Loop();

	void SetInputMode(int mode, int value) const;
	vec2 GetMousePosition() const;

	/**
	 * Runs the task on the UI thread at the start of the next frame. Once
	 * finished, the result will be available in the returned std::future.
	 */
	template<typename T>
	std::future<T> AddTask(Task<T> task) {
		// create the task and get its future
		auto t = std::make_unique<Task<T>>(std::move(task));
		std::future<T> future = t->GetFuture();

		{
			// add the task to the queue
			std::lock_guard lock(_mutex);
			_task_queue.push(std::move(t));
		}

		return future;
	}

private:
	Game& _game;

	std::queue<std::unique_ptr<TaskBase>> _task_queue;
	std::mutex _mutex;

private:
	static window_hints _create_window_hints(const DisplayConfiguration& configuration);

	friend void glfw_key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods);
	friend void glfw_char_callback(GLFWwindow* glfw_window, unsigned int codepoint);
	friend void glfw_mouse_move_callback(GLFWwindow* glfw_window, double xpos, double ypos);
	friend void glfw_mouse_button_callback(GLFWwindow* glfw_window, int button, int action, int mods);
	friend void glfw_scroll_callback(GLFWwindow* glfw_window, double x, double y);
	friend void glfw_window_focus_callback(GLFWwindow* glfw_window, int focus);

};

}

#endif
