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
#include <future>

namespace rheel {

class RE_API ThreadPool {

private:
	class TaskBase {

	public:
		virtual ~TaskBase() = default;
		virtual void operator()() = 0;

	};

	template<typename T>
	class Task : TaskBase {

	public:
		explicit Task(std::function<T()>&& task) :
				_task(std::forward<std::function<T()>>(task)) {}

		void operator()() override {
			_result.set_value(_task());
		}

		std::future<T> GetFuture() {
			return _result.get_future();
		}

	private:
		std::function<T()> _task;
		std::promise<T> _result;

	};

public:
	ThreadPool();
	~ThreadPool();

	/**
	 * Adds a task to the thread pool, to be executed by a background thread.
	 * Once finished, the result will be available in the returned std::future.
	 * Note: make sure that the task is thread-safe.
	 */
	template<typename T>
	std::future<T> AddTask(std::function<T()> task) {
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
	volatile bool _stop_requested = false;

private:
	static void ThreadMain_(ThreadPool* pool);

};

}

#endif
