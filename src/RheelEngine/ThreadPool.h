/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_THREADPOOL_H
#define RHEELENGINE_THREADPOOL_H
#include "_common.h"

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "Task.h"
#include "Renderer/Display/DummyWindow.h"

namespace rheel {

class RE_API ThreadPool {

public:
	ThreadPool();
	~ThreadPool();

	/**
	 * Adds a task to the thread pool, to be executed by a background thread.
	 * Once finished, the result will be available in the returned std::future.
	 * Note: make sure that the task is thread-safe.
	 */
	template<typename T>
	std::future<T> AddTask(Task<T> task) {
		// create the task and get its future
		auto t = std::make_unique<Task<T>>(std::move(task));
		std::future<T> future = t->GetFuture();

		{
			// add the task to the queue
			std::lock_guard lock(_queue_mutex);
			_execution_queue.push(std::move(t));

			// notify the next thread about the changed queue
			_queue_wait.notify_one();
		}

		return future;
	}

private:
	std::unique_ptr<TaskBase> GetNextTask_();

	std::vector<std::thread> _threads;
	std::queue<std::unique_ptr<TaskBase>> _execution_queue;
	std::mutex _queue_mutex;
	std::condition_variable _queue_wait;
	bool _stop_requested = false;

private:
	static void ThreadMain_(ThreadPool* pool, DummyWindow contextWindow);

};

}

#endif
