/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_FONTRENDERER_H
#define RHEELENGINE_FONTRENDERER_H
#include "../../_common.h"

#include "Font.h"
#include "PreparedText.h"
#include "../OpenGL/Framebuffer.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/Program.h"
#include "../../Color.h"

namespace rheel {

class RE_API FontRenderer {
	friend class TextRenderer;
	friend class PreparedText;
	friend class Game;

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
	void SetSize(unsigned size);
	void SetColor(Color color);

	const gl::VertexArray& GetCharacterVAO() const;
	const gl::Buffer& GetGlyphBuffer() const;

	void Render(const char32_t* text);
	void Render(const PreparedText& preparedText);

public:
	static constexpr inline unsigned SAMPLE_COUNT = 4;

private:
	FontRenderer(const Font& font);
	void Render_(const gl::VertexArray& vao, const gl::DrawElementsIndirectBuffer& indirectBuffer, const vec4& bounds, unsigned count);
	void ResizeBuffer_(unsigned width, unsigned height);

	const Font& _font;

	unsigned _size = 12;
	Color _color = 0xFFFFFF;

	gl::VertexArray _character_vao;
	gl::Buffer _glyph_buffer{ gl::Buffer::Target::ARRAY };
	gl::Buffer _transform_buffer{ gl::Buffer::Target::ARRAY };
	gl::DrawElementsIndirectBuffer _indirect_buffer;

private:
	static FontRenderer& Get_(const Font& font);

	static Cache<const Font*, FontRenderer> _renderers;

};

}

#endif
