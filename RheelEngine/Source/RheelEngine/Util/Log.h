/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef LOG_H_
#define LOG_H_

#ifndef RE_API
#error "Do not include Log.h directly, use _common.h"
#endif

#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <experimental/source_location>

// TODO: output to file.

namespace rheel {

using _SourceLoc = std::experimental::source_location;

class Log {

private:
	enum class _Level {
		INFO, WARNING, ERROR
	};

public:
	/**
	 * Returns the logging stream with the INFO level.
	 */
	static std::ostream& Info(const _SourceLoc& loc = _SourceLoc::current());

	/**
	 * Returns the logging stream with the WARNING level.
	 */
	static std::ostream& Warning(const _SourceLoc& loc = _SourceLoc::current());

	/**
	 * Returns the logging stream with the ERROR level.
	 */
	static std::ostream& Error(const _SourceLoc& loc = _SourceLoc::current());

private:
	static std::ostream& _Log(const _SourceLoc& loc, _Level level);

};

}

#endif
