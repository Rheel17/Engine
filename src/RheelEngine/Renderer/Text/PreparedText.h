/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_PREPAREDTEXT_H
#define RHEELENGINE_PREPAREDTEXT_H
#include "../../_common.h"

#include "Font.h"
#include "TextAlign.h"
#include "../OpenGL/DrawIndirectBuffer.h"

namespace rheel {

class RE_API PreparedText {

public:
	struct prepare_text_input {
		/* Text input */
		const char32_t* text;

		/* Text alignment */
		float width = std::numeric_limits<float>::max();
		TextAlign align{ TextAlign::LEFT };

		/* Font */
		std::reference_wrapper<const Font> font = std::ref(Font::GetDefaultFont());

		/* Tab width relative to the width of a single space. */
		float tab_width = 4.0f;

		/* Line height (vertical advance) relative to the font size. */
		float line_height = 1.1f;
	};

public:
	using DrawCommand = gl::DrawElementsIndirectBuffer::Command;

	static unsigned Prepare(const prepare_text_input& input, gl::Buffer& transformBuffer, gl::DrawElementsIndirectBuffer& indirectBuffer, vec4& bounds);

private:
	static unsigned Prepare_(const prepare_text_input& input, vec4& bounds);
	static void AlignLine_(size_t lineStart, size_t lineEnd, float space, TextAlign align);

	static std::vector<vec2> _transforms;
	static std::vector<DrawCommand> _commands;

};

}

#endif
