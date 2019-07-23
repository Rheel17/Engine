#ifndef RHEELENGINE_H_COMMON
#define RHEELENGINE_H_COMMON

#ifdef BUILDING_RHEELENGINE_DLL
#define RE_API __declspec(dllexport)
#else
#define RE_API __declspec(dllimport)
#endif

#include "Util/Math.h"

#define RE_NO_COPY(X) 					\
	X(const X&) = delete;				\
	X& operator=(const X&) = delete;

#define RE_NO_MOVE(X)					\
	X(X&&) = delete;					\
	X& operator=(X&&) = delete;

#endif
