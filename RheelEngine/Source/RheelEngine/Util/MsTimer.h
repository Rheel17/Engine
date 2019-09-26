/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef MSTIMER_H_
#define MSTIMER_H_
#include "../_common.h"

#include <chrono>

namespace rheel {

class RE_API MsTimer {

	using _TimePoint = std::chrono::_V2::system_clock::time_point;
	using _Duration = std::chrono::duration<float, std::milli>;

public:
	MsTimer(std::string name) :
		_start(std::chrono::high_resolution_clock::now()),
		_name(std::move(name)) {}

	~MsTimer() {
		std::cout << _name << ": " << Time() << " ms" << std::endl;
	}

	float Time() {
		_TimePoint end = std::chrono::high_resolution_clock::now();
		_Duration duration = end - _start;
		return duration.count();
	}

private:
	_TimePoint _start;
	std::string _name;

};

}

#endif
