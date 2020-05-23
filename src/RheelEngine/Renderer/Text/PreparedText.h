/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_PREPAREDTEXT_H
#define RHEELENGINE_PREPAREDTEXT_H
#include "../../_common.h"

#include "Font.h"
#include "TextRenderer.h"
#include "../OpenGL/VertexArray.h"

namespace rheel {

class RE_API PreparedText {

public:
	struct prepare_text_input {
		/* Text input */
		const char32_t* text;
		size_t char_count;

		/* Text alignment */
		float width;
		TextRenderer::TextAlign align{ TextRenderer::TextAlign::LEFT };

		/* Font */
		Font& font = Font::GetDefaultFont();

		/* Tab width relative to the width of a single space. */
		float tab_width = 4.0f;

		/* Line height (vertical advance) relative to the font size. */
		float line_height = 1.1f;
	};

public:
	PreparedText(const prepare_text_input& input);

private:
	gl::VertexArray _vao;
	gl::Buffer _transform_buffer{ gl::Buffer::Target::ARRAY };
	gl::DrawElementsIndirectBuffer _indirect_buffer;

public:
	using DrawCommand = gl::DrawElementsIndirectBuffer::Command;

	static void Prepare(const prepare_text_input& input, gl::Buffer& transformBuffer, gl::DrawElementsIndirectBuffer& indirectBuffer);

private:
	static void Prepare_(const prepare_text_input& input);

	static std::vector<vec2> _transforms;
	static std::vector<DrawCommand> _commands;

};

}

#endif
