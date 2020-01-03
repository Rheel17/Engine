/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_H_COMMON
#define RHEELENGINE_H_COMMON

#ifdef BUILDING_RHEELENGINE_DLL
#define RE_API __declspec(dllexport)
#else
#define RE_API __declspec(dllimport)
#endif

#include "Util/Math.h"
#include "Util/Log.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#define RE_NO_CONSTRUCT(X)				\
	X() = delete;

#define RE_NO_COPY(X) 					\
	X(const X&) = delete;				\
	X& operator=(const X&) = delete;

#define RE_NO_MOVE(X)					\
	X(X&&) = delete;					\
	X& operator=(X&&) = delete;

#endif
