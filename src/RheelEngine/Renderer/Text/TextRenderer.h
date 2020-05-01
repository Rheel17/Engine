/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_TEXTRENDERER_H
#define RHEELENGINE_TEXTRENDERER_H

#include "../../_common.h"

#include "Font.h"
#include "../OpenGL/VertexArray.h"
#include "../OpenGL/Program.h"
#include "../OpenGL/Framebuffer.h"
#include "../../Color.h"

namespace rheel {

class RE_API TextRenderer {
	struct ogl_data {
		ogl_data();
		gl::Buffer triangle_buffer;
		gl::VertexArray vao;

		gl::Buffer resolve_vbo;
		gl::VertexArray resolve_vao;

		gl::Framebuffer text_buffer;
		gl::Program shader;
	};
	mutable pseudo_static_pointer<ogl_data> _ogl_data;

public:
	void DrawText(Font& font, const Color& color, const std::wstring& text, int x, int y, unsigned size) const;
	void DrawText(Font& font, const Color& color, const std::string& text, int x, int y, unsigned size) const;

private:
	void ResizeBuffer_(unsigned width, unsigned height) const;

	int DrawChars_(Font& font, const Color& color, const wchar_t* text, unsigned length, int x, int y, unsigned size) const;

	void DrawTriangles_(const std::vector<Character::Triangle>& triangles, const std::vector<Character::Triangle>& bezierCurves, vec2 multisampleOffset) const;

};

}

#endif
