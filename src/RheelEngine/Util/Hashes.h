/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_HASHES_H
#define RHEELENGINE_HASHES_H
#include "../_common.h"

#include <functional>

namespace rheel {

template<typename T>
inline constexpr std::size_t hashCombine(const std::size_t& s, const T& t) {
	return s ^ (std::hash<T>()(t) + 0x9e3779b9 + (s << 6) + (s >> 2));
}

template<typename T>
inline constexpr std::size_t hashAll(const T& t) {
	return hashCombine(17, t);
}

template<typename T, typename... Rest>
inline constexpr std::size_t hashAll(const T& first, const Rest& ... rest) {
	return hashCombine(hashAll(rest...), first);
}

}

namespace std {

template<typename T1, typename T2>
struct hash<std::pair<T1, T2>> {
	constexpr std::size_t operator()(const std::pair<T1, T2>& p) const {
		return rheel::hashAll(p.first, p.second);
	}
};

template<>
struct hash<ivec2> {
	constexpr std::size_t operator()(const ivec2& v) const {
		return rheel::hashAll(v.x, v.y);
	}
};

template<>
struct hash<vec2> {
	constexpr std::size_t operator()(const vec2& v) const {
		return rheel::hashAll(v.x, v.y);
	}
};

template<>
struct hash<vec3> {
	constexpr std::size_t operator()(const vec3& v) const {
		return rheel::hashAll(v.x, v.y, v.z);
	}
};

template<>
struct hash<vec4> {
	constexpr std::size_t operator()(const vec4& v) const {
		return rheel::hashAll(v.x, v.y, v.z, v.w);
	}
};

}

#endif
