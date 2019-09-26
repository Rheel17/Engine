/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Character.h"

#include <algorithm>

namespace rheel {

Character::_ContourPoint::operator vec2() const {
	return { x, y };
}

Character::Character(const FT_GlyphSlot& glyph, unsigned short em) {
	_LoadTriangles(glyph->outline, float(em));
	_advance = glyph->advance.x / float(em);
}

const std::vector<Character::Triangle>& Character::Triangles() const {
	return _triangles;
}

const std::vector<Character::Triangle>& Character::BezierCurveTriangles() const {
	return _bezier_curves;
}

float Character::Advance() const {
	return _advance;
}

void Character::_LoadTriangles(const FT_Outline& outline, float em) {
	if (outline.n_contours == 0 || outline.n_points == 0) {
		return;
	}

	// calculate a common point to the bottom-left of all control points
	FT_Pos xMin = outline.points[0].x;
	FT_Pos yMin = outline.points[0].y;

	for (short i = 0; i < outline.n_points; i++) {
		xMin = std::min(xMin, outline.points[i].x);
		yMin = std::min(yMin, outline.points[i].y);
	}

	_common = { (xMin - 512) / em, (yMin - 512) / em };

	// add the contours
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

void Character::_AddContour(const _Contour& contour, float em) {
	// add the contour
	unsigned startIndex = 0;

	for (unsigned i = 1; i < contour.size(); i++) {
		if (contour[i].on) {
			vec2 v1 = (vec2) contour[startIndex] / em;
			vec2 v2 = (vec2) contour[i] / em;

			_triangles.push_back(_CreateTriangle(_common, v1, v2));

			// if the previous was more than 1 less than this, we had an 'off'
			// point in between, so add the Bézier curve.
			if (i - startIndex > 1) {
				_bezier_curves.push_back(_CreateTriangle(v1, (vec2) contour[i - 1] / em, v2));
			}

			// start the new triangle/curve at the current 'on' point.
			startIndex = i;
		}
	}
}

Character::Triangle Character::_CreateTriangle(const vec2& v1, const vec2& v2, const vec2& v3) {
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
