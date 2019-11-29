/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "../_common.h"

#include <string_format.h>

namespace rheel {

std::ostream& Log::Info(const _SourceLoc& loc) {
	return _Log(loc, _Level::INFO);
}

std::ostream& Log::Warning(const _SourceLoc& loc) {
	return _Log(loc, _Level::WARNING);
}

std::ostream& Log::Error(const _SourceLoc& loc) {
	return _Log(loc, _Level::ERROR);
}

std::ostream& Log::_Log(const _SourceLoc& loc, _Level level) {
	auto now = std::chrono::system_clock::now();
	auto millis = int(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000);
	std::time_t tm = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << std::put_time(std::gmtime(&tm), "%Y-%m-%d %H:%M:%S:");

	if (millis / 100 == 0) {
		ss << '0';

		if (millis / 10 == 0) {
			ss << '0';
			ss << millis;
		} else {
			ss << millis;
		}
	} else {
		ss << millis;
	}

	switch (level) {
		case _Level::INFO:
			ss << " [INFO]    ";
			break;
		case _Level::WARNING:
			ss << " [WARNING] ";
			break;
		case _Level::ERROR:
			ss << " [ERROR]   ";
			break;
	}

	return std::cout << ss.str();
}

}
