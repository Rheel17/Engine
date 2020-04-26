/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "ThreadPool.h"

namespace rheel {

ThreadPool::ThreadPool() {
	unsigned numThreads = std::thread::hardware_concurrency();
	_threads.reserve(numThreads);

	for (unsigned i = 0; i < numThreads; i++) {
		_threads.emplace_back(ThreadMain_, this);
	}

	Log::Info() << "Thread pool: " << numThreads << " threads" << std::endl;
}

ThreadPool::~ThreadPool() {
	// clear the execution queue
	while (!_execution_queue.empty()) {
		_execution_queue.pop();
	}

	// wait for the threads to finish
	for (std::thread& t : _threads) {
		t.join();
	}
}

void ThreadPool::ThreadMain_(ThreadPool* pool) {

}

}