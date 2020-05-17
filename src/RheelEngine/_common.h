/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE__COMMON_H
#define RHEELENGINE__COMMON_H

#if defined(_WIN32)
#define RE_API_EXPORT __declspec(dllexport)
#define RE_API_IMPORT __declspec(dllimport)
#elif defined(__linux__)
#define RE_API_EXPORT __attribute__((visibility("default")))
#define RE_API_IMPORT
#else
#error "Platform not supported"
#endif

#ifdef BUILDING_RHEELENGINE_DLL
#define RE_API RE_API_EXPORT
#else
#define RE_API RE_API_IMPORT
#endif

#include "Util/Math.h"
#include "Util/Log.h"
#include "Util/pseudo_static_pointer.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#define RE_NO_CONSTRUCT(X)                \
    X() = delete;

#define RE_NO_COPY(X)                     \
    X(const X&) = delete;                 \
    X& operator=(const X&) = delete;

#define RE_NO_MOVE(X)                     \
    X(X&&) = delete;                      \
    X& operator=(X&&) = delete;

#endif
