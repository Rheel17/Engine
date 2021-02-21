/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_LOG_H
#define RHEELENGINE_LOG_H

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

using SourceLoc = std::experimental::source_location;

class RE_API Log {

private:
	enum class Level {
		INFO, WARNING, ERROR
	};

public:
	/**
	 * Returns the logging stream with the INFO level.
	 */
	static std::ostream& Info(const SourceLoc& loc = SourceLoc::current());

	/**
	 * Returns the logging stream with the WARNING level.
	 */
	static std::ostream& Warning(const SourceLoc& loc = SourceLoc::current());

	/**
	 * Returns the logging stream with the ERROR level.
	 */
	static std::ostream& Error(const SourceLoc& loc = SourceLoc::current());

private:
	static std::ostream& _log(const SourceLoc& loc, Level level);

};

}

#endif
