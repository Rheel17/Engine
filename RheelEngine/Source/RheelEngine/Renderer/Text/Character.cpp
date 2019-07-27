#include "Character.h"

#include <algorithm>

namespace rheel {

Character::_ContourPoint::operator vec2() const {
	return { x, y };
}

Character::Character(const FT_GlyphSlot& glyph, unsigned short em) {
	std::cout << "em=" << em << std::endl;
	_LoadTriangles(glyph->outline, em);
	_advance = glyph->advance.x;
}

const std::vector<Character::Triangle>& Character::Triangles() const {
	return _triangles;
}

const std::vector<Character::Triangle>& Character::BezierCurveTriangles() const {
	return _bezier_curves;
}

void Character::_LoadTriangles(const FT_Outline& outline, unsigned short em) {
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

		_AddContour(contour, em);
		contourStart = contourEnd;
	}
}

void Character::_AddContour(const _Contour& contour, unsigned short emUnits) {
	float em = emUnits;

	// calculate a point to the left of all control points
	FT_Pos xMin = contour[0].x;
	FT_Pos yMin = contour[0].y;

	for (unsigned i = 0; i < contour.size(); i++) {
		std::cout << "  point " << contour[i].x << "," << contour[i].y;

		if (contour[i].on) {
			std::cout << " ON" << std::endl;
		} else {
			std::cout << " OFF" << std::endl;
		}

		xMin = std::min(xMin, contour[i].x);
		yMin = std::min(yMin, contour[i].y);
	}

	vec2 common = { (xMin - 512) / em, (yMin - 512) / em };

	// add the contour
	unsigned startIndex = 0;

	for (unsigned i = 1; i < contour.size(); i++) {
		if (contour[i].on) {
			vec2 v1 = (vec2) contour[startIndex] / em;
			vec2 v2 = (vec2) contour[i] / em;

			std::cout << "  " << common << " " << v1 << " " << v2 << std::endl;

			_triangles.push_back({{ common, v1, v2 }});
			_EnsureCounterClockwise(_triangles.back());

			// if the previous was more than 1 less than this, we had an 'off'
			// point in between, so add the Bézier curve.
			if (i - startIndex < 1) {
				_bezier_curves.push_back({{ v1, (vec2) contour[i - 1] / em, v2 }});
				_EnsureCounterClockwise(_bezier_curves.back());
			}

			// start the new triangle/curve at the current 'on' point.
			startIndex = i;
		}
	}
}

void Character::_EnsureCounterClockwise(Triangle& triangle) {
	vec2 u = triangle[1] - triangle[0];
	vec2 v = triangle[2] - triangle[0];

	// vertex 3 is on the right side of the 1 -> 2 vector, so it should come
	// before vertex 2.
	if (glm::dot(u, v) < 0) {
		vec2 tmp = std::move(triangle[1]);
		triangle[1] = std::move(triangle[2]);
		triangle[2] = std::move(tmp);
	}
}

}
