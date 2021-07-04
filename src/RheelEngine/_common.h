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


#ifdef RE_BUILDING_LIBRARY
// #define RE_API RE_API_EXPORT
#else
//#define RE_API RE_API_IMPORT
#endif

#define RE_API

#include "Util/Math.h"
#include "Util/Log.h"
#include "Util/pseudo_static_pointer.h"

#include <string>
#include <vector>
#include <memory>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>

#define RE_NO_CONSTRUCT(X)                \
    X() = delete

#define RE_NO_COPY(X)                     \
    X(const X&) = delete;                 \
    X& operator=(const X&) = delete

#define RE_NO_MOVE(X)                     \
    X(X&&) = delete;                      \
    X& operator=(X&&) = delete

#define RE_DEFAULT_COPY(X)                \
    X(const X&) = default;                \
    X& operator=(const X&) = default

#define RE_DEFAULT_MOVE(X)                \
    X(X&&) noexcept = default;            \
    X& operator=(X&&) noexcept = default

#endif
