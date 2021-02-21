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

Glyph::Triangle Glyph::_create_triangle(const vec2& v_1, const vec2& v_2, const vec2& v_3) {
	vec2 u = v_2 - v_1;
	vec2 v = v_3 - v_1;

	// calculate the determinant of the matrix [u v]. The sign of y will
	// determine the side of the third point, and thus if a flip is needed.
	if (u.y * v.x - u.x * v.y > 0) {
		return Triangle {{ vec3(v_1, -1.0f), vec3(v_3, -1.0f), vec3(v_2, -1.0f) }};
	} else {
		return Triangle {{ vec3(v_1, -1.0f), vec3(v_2, -1.0f), vec3(v_3, -1.0f) }};
	}
}

Glyph::Triangle Glyph::_create_bezier(const vec2& v_1, const vec2& v_2, const vec2& v_3) {
	vec2 u = v_2 - v_1;
	vec2 v = v_3 - v_1;

	// the barycentric coordinate index of the vertices is the same as the
	// defined order, and not necessarily the same as the order in the
	// Triangle type.

	// calculate the determinant of the matrix [u v]. The sign of y will
	// determine the side of the third point, and thus if a flip is needed.
	if (u.y * v.x - u.x * v.y > 0) {
		return Triangle {{ vec3(v_1, 0.0f), vec3(v_3, 1.0f), vec3(v_2, 2.0f) }};
	} else {
		return Triangle {{ vec3(v_1, 0.0f), vec3(v_2, 2.0f), vec3(v_3, 1.0f) }};
	}
}

}
