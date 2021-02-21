/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "PreparedText.h"

#include "FontRenderer.h"

namespace rheel {

std::vector<vec2> PreparedText::_transforms;
std::vector<PreparedText::DrawCommand> PreparedText::_commands;
std::vector<vec4> PreparedText::_bounds;

unsigned PreparedText::Prepare(const prepare_text_input& input, gl::Buffer& transform_buffer, gl::DrawElementsIndirectBuffer& indirect_buffer, vec4& bounds) {
	// prepare the internal buffers
	_transforms.clear();
	_commands.clear();
	_bounds.clear();

	// do the preparations
	unsigned count = _prepare(input);
	bounds = _combine_bounds();

	// upload the results to the OpenGL buffers
	transform_buffer.SetData(_transforms, gl::Buffer::Usage::STREAM_DRAW);
	indirect_buffer.SetData(_commands, gl::Buffer::Usage::STREAM_DRAW);

	return count;
}

unsigned PreparedText::_prepare(const prepare_text_input& input) {
	std::u32string_view text = input.text;

	float space_width = input.font.get().CharacterWidth(U' ');

	float px = 0.0f;
	float py = 0.0f;

	unsigned draw_index = 0;
	unsigned start_line = 0;
	unsigned word_boundary = 0;
	float word_boundary_px = 0.0f;

	for (size_t i = 0; i < text.length(); i++) {
		char32_t c = text[i];

		if (c == U'\n') {
			_align_line(start_line, draw_index, input.width - px, input.align);

			start_line = draw_index;
			word_boundary = draw_index;
			word_boundary_px = px;

			px = 0.0f;
			py -= input.line_height;
			continue;
		}

		if (c == U' ') {
			px += space_width;
			word_boundary = draw_index;
			word_boundary_px = px; // TOOD: set this to the first non-space non-tab character
			continue;
		}

		if (c == U'\t') {
			px = (std::floor(px / (space_width * input.tab_width)) + 1.0f) * (space_width * input.tab_width);
			word_boundary = draw_index;
			word_boundary_px = px; // TOOD: set this to the first non-space non-tab character
			continue;
		}

		// handle normal character
		size_t glyph_index = input.font.get().GetGlyphIndex(c);
		const auto& glyph = input.font.get().GetGlyph(glyph_index);
		const auto& [glyph_offset, glyph_size] = input.font.get().GetGlyphOffset(glyph_index);
		float advance = glyph.Advance();

		if (px + advance > input.width && start_line != word_boundary && draw_index > word_boundary) {
			_align_line(start_line, word_boundary, input.width - word_boundary_px, input.align);

			px = 0.0f;
			py -= input.line_height;

			unsigned remove = draw_index - word_boundary;
			draw_index = word_boundary;
			start_line = word_boundary;

			_transforms.erase(_transforms.end() - remove, _transforms.end());
			_commands.erase(_commands.end() - remove, _commands.end());
			i -= (remove + 1);

			continue;
		}

		_transforms.emplace_back(px, py);

		_commands.push_back(gl::DrawElementsIndirectBuffer::Command{
			.count = glyph_size,
			.instance_count = 4,
			.first_index = glyph_offset,
			.base_vertex = 0,
			.base_instance = draw_index++
		});

		_bounds.push_back(glyph.Bounds());

		px += advance;
	}

	_align_line(start_line, draw_index, input.width - px, input.align);

	return draw_index;
}

void PreparedText::_align_line(size_t line_start, size_t line_end, float space, TextAlign align) {
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

	for (size_t i = line_start; i < line_end; i++) {
		_transforms[i].x += shift;
	}
}

vec4 PreparedText::_combine_bounds() {
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
