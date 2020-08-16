/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_GLYPH_H
#define RHEELENGINE_GLYPH_H
#include "../../_common.h"

#include <freetype2/ft2build.h>
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
	float Advance() const;
	const vec4& Bounds() const;

private:
	template<typename Alloc>
	Glyph(const FT_GlyphSlot& glyph, unsigned short em, std::vector<Glyph::Triangle, Alloc>& triangles, std::vector<Glyph::Triangle, Alloc>& beziers) {
		LoadTriangles_(glyph->outline, float(em), triangles, beziers);
		_advance = glyph->advance.x / float(em);
		_bounds = vec4(
				glyph->metrics.horiBearingX,
				glyph->metrics.horiBearingY - glyph->metrics.height,
				glyph->metrics.horiBearingX + glyph->metrics.width,
				glyph->metrics.horiBearingY
		) / float(em);
	}

	template<typename Alloc>
	void LoadTriangles_(const FT_Outline& outline, float em, std::vector<Glyph::Triangle, Alloc>& triangles, std::vector<Glyph::Triangle, Alloc>& beziers) {
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

		vec2 common = { (xMin - 512) / em, (yMin - 512) / em };

		// add the contours
		unsigned contourStart = 0;
		unsigned contourEnd;

		for (short i = 0; i < outline.n_contours; i++) {
			contourEnd = outline.contours[i] + 1;
			Contour contour;

			// add contour points
			for (unsigned j = contourStart; j < contourEnd; j++) {
				contour_point point{};
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

			AddContour_(contour, common, em, triangles, beziers);
			contourStart = contourEnd;
		}
	}

	template<typename Alloc>
	void AddContour_(const Contour& contour, vec2 common, float em, std::vector<Glyph::Triangle, Alloc>& triangles, std::vector<Glyph::Triangle, Alloc>& beziers) {
		// add the contour
		unsigned startIndex = 0;

		for (unsigned i = 1; i < contour.size(); i++) {
			if (contour[i].on) {
				vec2 v1 = (vec2) contour[startIndex] / em;
				vec2 v2 = (vec2) contour[i] / em;

				triangles.push_back(CreateTriangle_(common, v1, v2));

				// if the previous was more than 1 less than this, we had an 'off'
				// point in between, so add the Bézier curve.
				if (i - startIndex > 1) {
					beziers.push_back(CreateBezier_(v1, (vec2) contour[i - 1] / em, v2));
				}

				// start the new triangle/curve at the current 'on' point.
				startIndex = i;
			}
		}
	}

	static Triangle CreateTriangle_(const vec2& v1, const vec2& v2, const vec2& v3);
	static Triangle CreateBezier_(const vec2& v1, const vec2& v2, const vec2& v3);

	float _advance;
	vec4 _bounds;

};

}

#endif
