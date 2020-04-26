/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_THREADPOOL_H
#define RHEELENGINE_THREADPOOL_H
#include "_common.h"

#include <queue>
#include <thread>

namespace rheel {

class RE_API ThreadPool {

public:
	ThreadPool();
	~ThreadPool();

private:
	std::vector<std::thread> _threads;
	std::queue<std::function<void()>> _execution_queue;

private:
	static void ThreadMain_(ThreadPool* pool);

};

}

#endif
