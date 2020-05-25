/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "PreparedText.h"

#include "FontRenderer.h"

namespace rheel {

std::vector<vec2> PreparedText::_transforms;
std::vector<PreparedText::DrawCommand> PreparedText::_commands;

Prepare::Prepare(const char* text) :
		_text(text) {}

Prepare::Prepare(const std::string& text) :
		_text(text.c_str()) {}

Prepare& Prepare::SetWidth(float width) {
	_width = width;
	return *this;
}

Prepare& Prepare::SetAlign(TextAlign align) {
	_align = align;
	return *this;
}

Prepare& Prepare::SetFont(const Font& font) {
	_font = std::ref(font);
	return *this;
}

PreparedText::PreparedText(const PreparedText::prepare_text_input& input) :
		_font(input.font) {

	FontRenderer& fontRenderer = FontRenderer::Get_(input.font);

	_vao.SetVertexAttributes<vec3>(fontRenderer.GetGlyphBuffer());
	_vao.SetVertexAttributes<vec2>(_transform_buffer, 0, FontRenderer::SAMPLE_COUNT);
	_vao.SetIndexBuffer(fontRenderer.GetCharacterVAO().GetIndexBuffer());

	_count = Prepare(input, _transform_buffer, _indirect_buffer, _bounds);
}

const Font& PreparedText::GetFont() const {
	return _font;
}

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

	while ((c = *(text++))) {
		if (c == U'\n') {
			px = 0.0f;
			py -= input.line_height;
			startLine = drawIndex;
			wordBoundary = drawIndex;
			continue;
		}

		if (c == U' ') {
			px += spaceWidth;
			wordBoundary = drawIndex;
			continue;
		}

		if (c == U'\t') {
			// TODO: handle tab
			wordBoundary = drawIndex;
			continue;
		}

		// handle normal character
		size_t glyphIndex = input.font.get().GetGlyphIndex(c);
		const auto& glyph = input.font.get().GetGlyph(glyphIndex);
		const auto& [glyphOffset, glyphSize] = input.font.get().GetGlyphOffset(glyphIndex);
		float advance = glyph.Advance();

		if (px + advance > input.width && startLine != wordBoundary && drawIndex > wordBoundary) {
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

	return drawIndex;
}

}
