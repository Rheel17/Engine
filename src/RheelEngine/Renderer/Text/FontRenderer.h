/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_FONTRENDERER_H
#define RHEELENGINE_FONTRENDERER_H
#include "../../_common.h"

#include "GlyphBuffer.h"
#include "../OpenGL/Framebuffer.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/Program.h"
#include "../../Color.h"

namespace rheel {

class RE_API FontRenderer {
	struct static_data {
		static_data();

		gl::Framebuffer text_buffer;

		gl::Program draw_program;
		gl::Program resolve_program;

		gl::VertexArray resolve_vao;
		gl::Buffer resolve_vbo;
	};

	pseudo_static_pointer<static_data> _static_data;

public:
	FontRenderer(Font& font);

	void SetSize(unsigned size);
	void SetColor(Color color);

	int Render(const char32_t** text, size_t count, int x, int y);

private:
	void ResizeBuffer_(unsigned width, unsigned height);

	Font& _font;

	unsigned _size = 12;
	Color _color = 0xFFFFFF;

	GlyphBuffer _glyph_buffer;
	gl::DrawElementsIndirectBuffer _indirect_buffer;

};

}

#endif
