#ifndef HASHES_H_
#define HASHES_H_
#include "../_common.h"

#include <functional>

namespace rheel {

template<typename T>
inline constexpr std::size_t hash_combine(const std::size_t& s, const T& t) {
	return s ^ (std::hash<T>()(t) + 0x9e3779b9 + (s << 6) + (s >> 2));
}

template<typename T>
inline constexpr std::size_t hash_all(const T& t) {
	return hash_combine(17, t);
}

template<typename T, typename... Rest>
inline constexpr std::size_t hash_all(const T& first, const Rest&... rest) {
	return hash_combine(hash_all(rest...), first);
}

}

namespace std {

	template<>
	struct RE_API hash<vec2> {
		constexpr std::size_t operator()(const vec2& v) const {
			return rheel::hash_all(v.x, v.y);
		}
	};

	template<>
	struct RE_API hash<vec3> {
		constexpr std::size_t operator()(const vec3& v) const {
			return rheel::hash_all(v.x, v.y, v.z);
		}
	};

	template<>
	struct RE_API hash<vec4> {
		constexpr std::size_t operator()(const vec4& v) const {
			return rheel::hash_all(v.x, v.y, v.z, v.w);
		}
	};
}

#endif
