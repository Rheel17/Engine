/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef TEXTRENDERER_H_
#define TEXTRENDERER_H_
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
	static void DrawText(Font& font, const Color& color, const std::wstring& text,
			int x, int y, unsigned size);

	static void DrawText(Font& font, const Color& color, const std::string& text,
			int x, int y, unsigned size);

private:
	static void _Initialize();
	static void _ResizeBuffer(unsigned width, unsigned height);

	static int _DrawChars(Font& font, const Color& color, const wchar_t *text, unsigned length,
			int x, int y, unsigned size);
	static void _DrawTriangles(const std::vector<Character::Triangle>& triangles,
			const std::vector<Character::Triangle>& bezierCurves, vec4 bounds,
			vec2 multisampleOffset);

	static GL::Buffer _triangle_buffer;
	static GL::VertexArray _vao;

	static GL::Buffer _resolve_vbo;
	static GL::VertexArray _resolve_vao;

	static std::unique_ptr<GL::Framebuffer> _text_buffer;
	static GL::Program _shader;

	static bool _initialized;
	static unsigned _width;
	static unsigned _height;

};

}

#endif
