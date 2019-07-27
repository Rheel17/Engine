#ifndef CHARACTER_H_
#define CHARACTER_H_
#include "../../_common.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <array>
#include <vector>

namespace rheel {

class RE_API Character {
	friend class Font;

private:
	struct _ContourPoint {
		FT_Pos x;
		FT_Pos y;
		bool on;

		explicit operator vec2() const;
	};

	using _Contour = std::vector<_ContourPoint>;

public:
	using Triangle = std::array<vec2, 3>;

public:
	const std::vector<Triangle>& Triangles() const;
	const std::vector<Triangle>& BezierCurveTriangles() const;

private:
	Character(const FT_GlyphSlot& glyph, unsigned short em);

	void _LoadTriangles(const FT_Outline& outline, unsigned short em);
	void _AddContour(const _Contour& contour, unsigned short em);
	void _EnsureCounterClockwise(Triangle& triangle);

	std::vector<Triangle> _triangles;
	std::vector<Triangle> _bezier_curves;
	float _advance;

};

}

#endif
