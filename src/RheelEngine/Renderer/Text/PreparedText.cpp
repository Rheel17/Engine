/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "PreparedText.h"

namespace rheel {

std::vector<vec2> PreparedText::_transforms;
std::vector<PreparedText::DrawCommand> PreparedText::_commands;

PreparedText::PreparedText(const PreparedText::prepare_text_input& input) {
	FontRenderer& fontRenderer = FontRenderer::Get_(input.font);

	_vao.SetVertexAttributes<vec3>(fontRenderer.GetGlyphBuffer());
	_vao.SetVertexAttributes<vec2>(_transform_buffer, 0, FontRenderer::SAMPLE_COUNT);
	_vao.SetIndexBuffer(fontRenderer.GetCharacterVAO().GetIndexBuffer());

	Prepare(input, _transform_buffer, _indirect_buffer);
}

void PreparedText::Prepare(const prepare_text_input& input, gl::Buffer& transformBuffer, gl::DrawElementsIndirectBuffer& indirectBuffer) {
	_transforms.clear();
	_commands.clear();

	Prepare_(input);

	transformBuffer.SetData(_transforms);
	indirectBuffer.SetData(_commands);
}

void PreparedText::Prepare_(const prepare_text_input& input) {
	const char32_t* text = input.text;
	char32_t c;

	float px = 0.0f;
	float py = 0.0f;

	unsigned drawIndex = 0;

	while ((c = *(text++))) {
		if (c == U'\n') {
			// handle newline
			// continue;
		}

		if (c == U' ') {
			// handle space
			// continue;
		}

		if (c == U'\t') {
			// handle tab
			// continue;
		}

		// handle normal character
		size_t glyphIndex = input.font.GetGlyphIndex(c);
		const auto& glyph = input.font.GetGlyph(glyphIndex);
		const auto& [glyphOffset, glyphSize] = input.font.GetGlyphOffset(glyphIndex);
		float advance = glyph.Advance();

		_transforms.emplace_back(px, py);
		px += advance;

		_commands.push_back(gl::DrawElementsIndirectBuffer::Command{
			.count = glyphSize,
			.instance_count = 4,
			.first_index = glyphOffset,
			.base_vertex = 0,
			.base_instance = drawIndex++
		});
	}
}

}
