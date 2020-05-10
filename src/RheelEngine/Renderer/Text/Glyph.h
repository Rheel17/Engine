/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_GLYPH_H
#define RHEELENGINE_GLYPH_H
#include "../../_common.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <array>

namespace rheel {

class RE_API Glyph {
	friend class Font;

private:
	struct contour_point {
		FT_Pos x;
		FT_Pos y;
		bool on;

		explicit operator vec2() const;
	};

	using Contour = std::vector<contour_point>;

public:
	using Triangle = std::array<vec3, 3>;

public:
	const std::vector<Triangle>& Triangles() const;
	const std::vector<Triangle>& BezierCurveTriangles() const;
	float Advance() const;

private:
	Glyph(const FT_GlyphSlot& glyph, unsigned short em);

	void LoadTriangles_(const FT_Outline& outline, float em);
	void AddContour_(const Contour& contour, float em);

	static Triangle CreateTriangle_(const vec2& v1, const vec2& v2, const vec2& v3);
	static Triangle CreateBezier_(const vec2& v1, const vec2& v2, const vec2& v3);

	vec2 _common{};
	std::vector<Triangle> _triangles;
	std::vector<Triangle> _bezier_curves;
	float _advance;

};

}

#endif
