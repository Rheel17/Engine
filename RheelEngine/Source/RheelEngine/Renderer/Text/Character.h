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

		explicit operator FT_Vector() const;
	};

	using _Contour = std::vector<_ContourPoint>;

public:
	using Triangle = std::array<FT_Vector, 3>;

private:
	Character(const FT_GlyphSlot& glyph);

	void _LoadTriangles(const FT_Outline& outline);
	void _AddContour(const _Contour& contour);

	std::vector<Triangle> _triangles;
	std::vector<Triangle> _bezier_curves;

};

}

#endif
