/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef TEXTRENDERER_H_
#define TEXTRENDERER_H_
#include "../../_common.h"

#include "Font.h"
#include "../_OpenGL/_GLVertexArray.h"
#include "../_OpenGL/_GLShaderProgram.h"
#include "../_OpenGL/_GLFramebuffer.h"
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
			const std::vector<Character::Triangle>& bezierCurves,
			vec2 multisampleOffset);

	static std::unique_ptr<_GLBuffer> _triangle_buffer;
	static std::unique_ptr<_GLVertexArray> _vao;

	static std::unique_ptr<_GLBuffer> _resolve_vbo;
	static std::unique_ptr<_GLVertexArray> _resolve_vao;

	static std::unique_ptr<_GLFramebuffer> _text_buffer;
	static _GLShaderProgram _shader;

	static bool _initialized;
	static unsigned _width;
	static unsigned _height;

};

}

#endif
