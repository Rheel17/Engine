/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Glyph.h"

namespace rheel {

Glyph::contour_point::operator vec2() const {
	return { x, y };
}

float Glyph::Advance() const {
	return _advance;
}

const vec4& Glyph::Bounds() const {
	return _bounds;
}

Glyph::Triangle Glyph::CreateTriangle_(const vec2& v1, const vec2& v2, const vec2& v3) {
	vec2 u = v2 - v1;
	vec2 v = v3 - v1;

	// calculate the determinant of the matrix [u v]. The sign of y will
	// determine the side of the third point, and thus if a flip is needed.
	if (u.y * v.x - u.x * v.y > 0) {
		return Triangle {{ vec3(v1, -1.0f), vec3(v3, -1.0f), vec3(v2, -1.0f) }};
	} else {
		return Triangle {{ vec3(v1, -1.0f), vec3(v2, -1.0f), vec3(v3, -1.0f) }};
	}
}

Glyph::Triangle Glyph::CreateBezier_(const vec2& v1, const vec2& v2, const vec2& v3) {
	vec2 u = v2 - v1;
	vec2 v = v3 - v1;

	// the barycentric coordinate index of the vertices is the same as the
	// defined order, and not necessarily the same as the order in the
	// Triangle type.

	// calculate the determinant of the matrix [u v]. The sign of y will
	// determine the side of the third point, and thus if a flip is needed.
	if (u.y * v.x - u.x * v.y > 0) {
		return Triangle {{ vec3(v1, 0.0f), vec3(v3, 1.0f), vec3(v2, 2.0f) }};
	} else {
		return Triangle {{ vec3(v1, 0.0f), vec3(v2, 2.0f), vec3(v3, 1.0f) }};
	}
}

}
