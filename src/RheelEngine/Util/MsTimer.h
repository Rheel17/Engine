/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_MSTIMER_H
#define RHEELENGINE_MSTIMER_H
#include "../_common.h"

#include <chrono>

namespace rheel {

class RE_API MsTimer {
	using TimePoint = std::chrono::_V2::system_clock::time_point;
	using Duration = std::chrono::duration<float, std::milli>;

public:
	MsTimer(std::string name) :
			_start(std::chrono::high_resolution_clock::now()),
			_name(std::move(name)) {}

	~MsTimer() {
		std::cout << _name << ": " << Time() << " ms" << std::endl;
	}

	float Time() {
		TimePoint end = std::chrono::high_resolution_clock::now();
		Duration duration = end - _start;
		return duration.count();
	}

private:
	TimePoint _start;
	std::string _name;

};

}

#endif
