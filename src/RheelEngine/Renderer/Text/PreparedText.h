/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_PREPAREDTEXT_H
#define RHEELENGINE_PREPAREDTEXT_H
#include "../../_common.h"

#include "Font.h"
#include "TextAlign.h"
#include "../OpenGL/VertexArray.h"

namespace rheel {

class RE_API Prepare {
	friend class TextRenderer;

public:
	Prepare(const char* text);
	Prepare(const std::string& text);

	Prepare& SetWidth(float width);
	Prepare& SetAlign(TextAlign align);
	Prepare& SetFont(const Font& font);

private:
	const char* _text;
	float _width = std::numeric_limits<float>::max();
	TextAlign _align = TextAlign::LEFT;
	std::reference_wrapper<const Font> _font = std::ref(Font::GetDefaultFont());

};

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
	PreparedText(const prepare_text_input& input);

	const Font& GetFont() const;

private:
	const Font& _font;

	gl::VertexArray _vao;
	gl::Buffer _transform_buffer{ gl::Buffer::Target::ARRAY };
	gl::DrawElementsIndirectBuffer _indirect_buffer;

	unsigned _count;
	vec4 _bounds;

public:
	using DrawCommand = gl::DrawElementsIndirectBuffer::Command;

	static unsigned Prepare(const prepare_text_input& input, gl::Buffer& transformBuffer, gl::DrawElementsIndirectBuffer& indirectBuffer, vec4& bounds);

private:
	static unsigned Prepare_(const prepare_text_input& input, vec4& bounds);

	static std::vector<vec2> _transforms;
	static std::vector<DrawCommand> _commands;

};

}

#endif
