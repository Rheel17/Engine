/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "PreparedText.h"

#include "FontRenderer.h"

namespace rheel {

std::vector<vec2> PreparedText::_transforms;
std::vector<PreparedText::DrawCommand> PreparedText::_commands;
std::vector<vec4> PreparedText::_bounds;

unsigned PreparedText::Prepare(const prepare_text_input& input, gl::Buffer& transformBuffer, gl::DrawElementsIndirectBuffer& indirectBuffer, vec4& bounds) {
	// prepare the internal buffers
	_transforms.clear();
	_commands.clear();
	_bounds.clear();

	// do the preparations
	unsigned count = Prepare_(input);
	bounds = CombineBounds_();

	// upload the results to the OpenGL buffers
	transformBuffer.SetData(_transforms);
	indirectBuffer.SetData(_commands);

	return count;
}

unsigned PreparedText::Prepare_(const prepare_text_input& input) {
	const char32_t* text = input.text;
	char32_t c;



	float spaceWidth = input.font.get().CharacterWidth(U' ');

	float px = 0.0f;
	float py = 0.0f;

	unsigned drawIndex = 0;
	unsigned startLine = 0;
	unsigned wordBoundary = 0;
	float wordBoundaryPx = 0.0f;

	while ((c = *(text++))) {
		if (c == U'\n') {
			AlignLine_(startLine, drawIndex, input.width - px, input.align);

			startLine = drawIndex;
			wordBoundary = drawIndex;
			wordBoundaryPx = px;

			px = 0.0f;
			py -= input.line_height;
			continue;
		}

		if (c == U' ') {
			px += spaceWidth;
			wordBoundary = drawIndex;
			wordBoundaryPx = px; // TOOD: set this to the first non-space non-tab character
			continue;
		}

		if (c == U'\t') {
			px = (std::floor(px / (spaceWidth * input.tab_width)) + 1.0f) * (spaceWidth * input.tab_width);
			wordBoundary = drawIndex;
			wordBoundaryPx = px; // TOOD: set this to the first non-space non-tab character
			continue;
		}

		// handle normal character
		size_t glyphIndex = input.font.get().GetGlyphIndex(c);
		const auto& glyph = input.font.get().GetGlyph(glyphIndex);
		const auto& [glyphOffset, glyphSize] = input.font.get().GetGlyphOffset(glyphIndex);
		float advance = glyph.Advance();

		if (px + advance > input.width && startLine != wordBoundary && drawIndex > wordBoundary) {
			AlignLine_(startLine, wordBoundary, input.width - wordBoundaryPx, input.align);

			px = 0.0f;
			py -= input.line_height;

			unsigned remove = drawIndex - wordBoundary;
			drawIndex = wordBoundary;
			startLine = wordBoundary;

			_transforms.erase(_transforms.end() - remove, _transforms.end());
			_commands.erase(_commands.end() - remove, _commands.end());
			text -= (remove + 1);

			continue;
		}

		_transforms.emplace_back(px, py);

		_commands.push_back(gl::DrawElementsIndirectBuffer::Command{
			.count = glyphSize,
			.instance_count = 4,
			.first_index = glyphOffset,
			.base_vertex = 0,
			.base_instance = drawIndex++
		});

		_bounds.push_back(glyph.Bounds());

		px += advance;
	}

	AlignLine_(startLine, drawIndex, input.width - px, input.align);

	return drawIndex;
}

void PreparedText::AlignLine_(size_t lineStart, size_t lineEnd, float space, TextAlign align) {
	float shift = 0.0f;

	switch (align) {
		case TextAlign::LEFT:
			return;
		case TextAlign::RIGHT:
			shift = space;
			break;
		case TextAlign::CENTER:
			shift = space / 2.0f;
			break;
	}

	for (size_t i = lineStart; i < lineEnd; i++) {
		_transforms[i].x += shift;
	}
}

vec4 PreparedText::CombineBounds_() {
	if (_bounds.empty()) {
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	constexpr float fmin = std::numeric_limits<float>::lowest();
	constexpr float fmax = std::numeric_limits<float>::max();
	vec4 bounds = { fmax, fmax, fmin, fmin };

	for (size_t i = 0; i < _bounds.size(); i++) {
		const vec4& b = _bounds[i];
		const vec2& p = _transforms[i];

		bounds.x = std::min(bounds.x, b.x + p.x);
		bounds.y = std::min(bounds.y, b.y + p.y);
		bounds.z = std::max(bounds.z, b.z + p.x);
		bounds.w = std::max(bounds.w, b.w + p.y);
	}

	bounds.x -= 0.1f;
	bounds.y -= 0.1f;
	bounds.z += 0.1f;
	bounds.w += 0.1f;

	return bounds;
}

}
