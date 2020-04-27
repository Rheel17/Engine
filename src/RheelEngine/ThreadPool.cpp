/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "ThreadPool.h"

#include "Engine.h"

namespace rheel {

ThreadPool::ThreadPool() {
	// get the parent window handle
	auto mainWindow = static_cast<GLFWwindow*>(Engine::GetWindow().GetWindowHandle());

	// create the threads
	unsigned numThreads = std::thread::hardware_concurrency();
	_threads.reserve(numThreads);

	for (unsigned i = 0; i < numThreads; i++) {
		_threads.emplace_back(ThreadMain_, this, CreateWindowHandle_(mainWindow));
	}

	Log::Info() << "Thread pool: " << numThreads << " threads" << std::endl;
}

ThreadPool::~ThreadPool() {
	// send the stop signal to the threads
	{
		std::lock_guard lock(_queue_mutex);
		Log::Info() << "Stopping thread pool" << std::endl;

		_stop_requested = true;
	}

	_queue_wait.notify_all();

	// wait for the threads to finish
	for (std::thread& t : _threads) {
		t.join();
	}
}

std::unique_ptr<ThreadPool::TaskBase> ThreadPool::GetNextTask_() {
	// wait for a task to become available
	std::unique_lock lock(_queue_mutex);
	_queue_wait.wait(lock, [this]() { return _stop_requested || !_execution_queue.empty(); });

	// check if a stop was requested
	if (_stop_requested) {
		return nullptr;
	}

	// retrieve and remove the next task
	auto task = std::move(_execution_queue.front());
	_execution_queue.pop();
	return task;
}

void ThreadPool::ThreadMain_(ThreadPool* pool, void* contextWindow) {
	// ensure we have an OpenGL context
	auto window = static_cast<GLFWwindow*>(contextWindow);
	glfwMakeContextCurrent(window);

	while (true) {
		// fetch a task
		auto task = pool->GetNextTask_();

		// exit condition: GetNextTask_() didn't return a task
		if (!task) {
			break;
		}

		// execute the task
		task->operator()();
	}

	// destroy the OpenGL context
	glfwMakeContextCurrent(nullptr);
	glfwDestroyWindow(window);
}

void* ThreadPool::CreateWindowHandle_(void* mainWindow) {
	glfwWindowHint(GLFW_VISIBLE, false);
	GLFWwindow* dummyWindow = glfwCreateWindow(1, 1, "", nullptr, static_cast<GLFWwindow*>(mainWindow));

	if (dummyWindow == nullptr) {
		Log::Error() << "Failed to create dummy window for thread pool" << std::endl;
		abort();
	}

	return dummyWindow;
}

}
