#ifndef RHEELENGINE_H_COMMON
#define RHEELENGINE_H_COMMON

#ifdef BUILDING_RHEELENGINE_DLL
#define RE_API __declspec(dllexport)
#else
#define RE_API __declspec(dllimport)
#endif

#include "Util/Math.h"

#endif
