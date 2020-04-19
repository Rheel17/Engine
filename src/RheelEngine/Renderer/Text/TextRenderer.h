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
	friend class Engine;

	RE_NO_CONSTRUCT(TextRenderer)

public:
	static void DrawText(Font& font, const Color& color, const std::wstring& text, int x, int y, unsigned size);
	static void DrawText(Font& font, const Color& color, const std::string& text, int x, int y, unsigned size);

private:
	static void Initialize_();

	static void ResizeBuffer_(unsigned width, unsigned height);

	static int DrawChars_(Font& font, const Color& color, const wchar_t* text, unsigned length, int x, int y, unsigned size);

	static void DrawTriangles_(const std::vector<Character::Triangle>& triangles,
			const std::vector<Character::Triangle>& bezierCurves, vec2 multisampleOffset);

	static gl::Buffer _triangle_buffer;
	static gl::VertexArray _vao;

	static gl::Buffer _resolve_vbo;
	static gl::VertexArray _resolve_vao;

	static std::unique_ptr<gl::Framebuffer> _text_buffer;
	static gl::Program _shader;

	static bool _initialized;
	static unsigned _width;
	static unsigned _height;

};

}

#endif
