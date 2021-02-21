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
		_load_triangles(glyph->outline, float(em), triangles, beziers);
		_advance = glyph->advance.x / float(em);
		_bounds = vec4(
				glyph->metrics.horiBearingX,
				glyph->metrics.horiBearingY - glyph->metrics.height,
				glyph->metrics.horiBearingX + glyph->metrics.width,
				glyph->metrics.horiBearingY
		) / float(em);
	}

	template<typename Alloc>
	void _load_triangles(const FT_Outline& outline, float em, std::vector<Glyph::Triangle, Alloc>& triangles, std::vector<Glyph::Triangle, Alloc>& beziers) {
		if (outline.n_contours == 0 || outline.n_points == 0) {
			return;
		}

		// calculate a common point to the bottom-left of all control points
		FT_Pos x_min = outline.points[0].x;
		FT_Pos y_min = outline.points[0].y;

		for (short i = 0; i < outline.n_points; i++) {
			x_min = std::min(x_min, outline.points[i].x);
			y_min = std::min(y_min, outline.points[i].y);
		}

		vec2 common = { (x_min - 512) / em, (y_min - 512) / em };

		// add the contours
		unsigned contour_start = 0;
		unsigned contour_end;

		for (short i = 0; i < outline.n_contours; i++) {
			contour_end = outline.contours[i] + 1;
			Contour contour;

			// add contour points
			for (unsigned j = contour_start; j < contour_end; j++) {
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

			_add_contour(contour, common, em, triangles, beziers);
			contour_start = contour_end;
		}
	}

	template<typename Alloc>
	void _add_contour(const Contour& contour, vec2 common, float em, std::vector<Glyph::Triangle, Alloc>& triangles, std::vector<Glyph::Triangle, Alloc>& beziers) {
		// add the contour
		unsigned start_index = 0;

		for (unsigned i = 1; i < contour.size(); i++) {
			if (contour[i].on) {
				vec2 v_1 = (vec2) contour[start_index] / em;
				vec2 v_2 = (vec2) contour[i] / em;

				triangles.push_back(_create_triangle(common, v_1, v_2));

				// if the previous was more than 1 less than this, we had an 'off'
				// point in between, so add the Bézier curve.
				if (i - start_index > 1) {
					beziers.push_back(_create_bezier(v_1, (vec2) contour[i - 1] / em, v_2));
				}

				// start the new triangle/curve at the current 'on' point.
				start_index = i;
			}
		}
	}

	static Triangle _create_triangle(const vec2& v_1, const vec2& v_2, const vec2& v_3);
	static Triangle _create_bezier(const vec2& v_1, const vec2& v_2, const vec2& v_3);

	float _advance;
	vec4 _bounds;

};

}

#endif
