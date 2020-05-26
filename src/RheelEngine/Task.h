/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_TASK_H
#define RHEELENGINE_TASK_H
#include "_common.h"

#include <future>

class TaskBase {

public:
	virtual ~TaskBase() = default;
	virtual void operator()() = 0;

};

template<typename T>
class Task : public TaskBase {

public:
	template<typename Callable>
	Task(Callable&& task) :
			_task(std::forward<Callable>(task)) {}

	void operator()() override {
		Run_();
	}

	std::future<T> GetFuture() {
		return _result.get_future();
	}

private:
	void Run_() {
		if constexpr (std::is_same_v<T, void>) {
			_task();
			_result.set_value();
		} else {
			_result.set_value(_task());
		}
	}

	std::function<T()> _task;
	std::promise<T> _result;

};

#endif