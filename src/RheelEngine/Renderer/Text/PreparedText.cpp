/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "PreparedText.h"

#include "FontRenderer.h"

namespace rheel {

std::vector<vec2> PreparedText::_transforms;
std::vector<PreparedText::DrawCommand> PreparedText::_commands;

unsigned PreparedText::Prepare(const prepare_text_input& input, gl::Buffer& transformBuffer, gl::DrawElementsIndirectBuffer& indirectBuffer, vec4& bounds) {
	// prepare the internal buffers
	_transforms.clear();
	_commands.clear();

	// do the preparations
	unsigned count = Prepare_(input, bounds);

	// upload the results to the OpenGL buffers
	transformBuffer.SetData(_transforms);
	indirectBuffer.SetData(_commands);

	// add a border to the bounds
	bounds.x -= 0.1f;
	bounds.y -= 0.1f;
	bounds.z += 0.1f;
	bounds.w += 0.1f;

	return count;
}

unsigned PreparedText::Prepare_(const prepare_text_input& input, vec4& bounds) {
	const char32_t* text = input.text;
	char32_t c;

	constexpr float fmin = std::numeric_limits<float>::lowest();
	constexpr float fmax = std::numeric_limits<float>::max();
	bounds = { fmax, fmax, fmin, fmin };

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
			// TODO: handle tab
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

		const vec4& glyphBounds = glyph.Bounds();
		bounds.x = std::min(bounds.x, glyphBounds.x + px);
		bounds.y = std::min(bounds.y, glyphBounds.y + py);
		bounds.z = std::max(bounds.z, glyphBounds.z + px);
		bounds.w = std::max(bounds.w, glyphBounds.w + py);

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

}
