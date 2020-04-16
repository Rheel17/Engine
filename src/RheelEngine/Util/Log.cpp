/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "../_common.h"

#include <string_format.h>

namespace rheel {

std::ostream& Log::Info(const SourceLoc& loc) {
	return Log_(loc, Level::INFO);
}

std::ostream& Log::Warning(const SourceLoc& loc) {
	return Log_(loc, Level::WARNING);
}

std::ostream& Log::Error(const SourceLoc& loc) {
	return Log_(loc, Level::ERROR);
}

std::ostream& Log::Log_(const SourceLoc& loc, Level level) {
	auto now = std::chrono::system_clock::now();
	auto millis = int(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000);
	std::time_t tm = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << std::put_time(std::gmtime(&tm), "%Y-%m-%d %H:%M:%S.");

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
		case Level::INFO:
			ss << " [INFO]    ";
			break;
		case Level::WARNING:
			ss << " [WARNING] ";
			break;
		case Level::ERROR:
			ss << " [ERROR]   ";
			break;
	}

	return std::cout << ss.str();
}

}
