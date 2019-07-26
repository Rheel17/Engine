#include "Character.h"

#include <algorithm>

namespace rheel {

Character::_ContourPoint::operator FT_Vector_() const {
	return { x, y };
}

Character::Character(const FT_GlyphSlot& glyph) {
	_LoadTriangles(glyph->outline);
}

void Character::_LoadTriangles(const FT_Outline& outline) {
	std::cout << outline.n_contours << " contours, " << outline.n_points << " points" << std::endl;

	if (outline.n_contours == 0) {
		return;
	}

	unsigned contourStart = 0;
	unsigned contourEnd;

	for (short i = 0; i < outline.n_contours; i++) {
		contourEnd = outline.contours[i] + 1;
		_Contour contour;

		// add contour points
		for (unsigned j = contourStart; j < contourEnd; j++) {
			_ContourPoint point;
			point.x = outline.points[j].x;
			point.y = outline.points[j].y;

			switch (FT_CURVE_TAG(outline.tags[j])) {
				case FT_CURVE_TAG_ON:		point.on = true; break;
				case FT_CURVE_TAG_CONIC:	point.on = false; break;
				case FT_CURVE_TAG_CUBIC:
					throw std::runtime_error("Fonts containing cubic Bézier curves are not supported.");
			}

			// if the last point was an 'off' point and this is an 'off' point
			// too, add two 'on' points in between
			if (!contour.empty() && !point.on && !contour.back().on) {
				contour.push_back({
						(contour.back().x + point.x) / 2,
						(contour.back().y + point.y) / 2,
						true
				});
			}

			// add the point
			contour.push_back(point);
		}

		// if the contour starts and end with an 'off' point, add two 'on'
		// points in between
		if (!contour.front().on && !contour.back().on) {
			contour.push_back({
					(contour.front().x + contour.back().x) / 2,
					(contour.front().y + contour.back().y) / 2,
					true
			});
		}

		// if the contour starts with an 'off' point, rotate by 1 so that it
		// starts with an 'on' point.
		if (!contour.front().on) {
			std::rotate(contour.begin(), contour.begin() + 1, contour.end());
		}

		// add the first point to the back to close the contour
		contour.push_back(contour.front());

		_AddContour(contour);
		contourStart = contourEnd;
	}
}

void Character::_AddContour(const _Contour& contour) {
	// calculate a point to the left of all control points
	FT_Pos xMin = contour[0].x;

	for (unsigned i = 1; i < contour.size(); i++) {
		xMin = std::min(xMin, contour[i].x);
	}

	FT_Vector common = { xMin - 4096, 0 };

	// add the contour
	unsigned startIndex = 0;

	for (unsigned i = 1; i < contour.size(); i++) {
		if (contour[i].on) {
			FT_Vector v1 = (FT_Vector) contour[startIndex];
			FT_Vector v2 = (FT_Vector) contour[i];

			_triangles.push_back({{ common, v1, v2 }});

			// if the previous was more than 1 less than this, we had an 'off'
			// point in between, so add the Bézier curve.
			if (i - startIndex < 1) {
				_bezier_curves.push_back({{ v1, (FT_Vector) contour[i - 1], v2 }});
			}

			// start the new triangle/curve at the current 'on' point.
			startIndex = i;
		}
	}
}

}
