/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextRenderer.h"

#include <limits>

#include "../../Engine.h"
#include "../../EngineResources.h"
#include "../OpenGL/State.h"

#define STAGE_TRIANGLES		0
#define STAGE_BEZIER		1
#define STAGE_RESOLVE		2
#define STAGE_COPY          3

namespace rheel {

GL::Buffer TextRenderer::_triangle_buffer(GL::Buffer::Target::ARRAY);
GL::VertexArray TextRenderer::_vao;

GL::Buffer TextRenderer::_resolve_vbo(GL::Buffer::Target::ARRAY);
GL::VertexArray TextRenderer::_resolve_vao;

std::unique_ptr<GL::Framebuffer> TextRenderer::_text_buffer;
GL::Program TextRenderer::_shader;

bool TextRenderer::_initialized(false);
unsigned TextRenderer::_width(1);
unsigned TextRenderer::_height(1);

// This text rendering system is based on the amazing blog post by Evan Wallace:
// https://medium.com/@evanwallace/easy-scalable-text-rendering-on-the-gpu-c3f4d782c5ac

void TextRenderer::DrawText(Font& font, const Color& color, const std::wstring& text, int x, int y, unsigned size) {
	const wchar_t *chars = text.c_str();
	unsigned length = text.length();

	// draw a maximum of Font::NUM_GLYPHS characters at a time.
	while (length > 0) {
		unsigned charsLength = std::min(length, Font::FONT_CACHE_SIZE);

		x = _DrawChars(font, color, chars, charsLength, x, y, size);

		length -= charsLength;
		chars += charsLength;
	}
}

void TextRenderer::DrawText(Font& font, const Color& color, const std::string& text, int x, int y, unsigned size) {
	std::wstring wide;
	wide.assign(text.begin(), text.end());

	DrawText(font, color, wide, x, y, size);
}

void TextRenderer::_Initialize() {
	if (_initialized) {
		return;
	}

	_shader.AttachShader(GL::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_fontshader_frag_glsl"));
	_shader.AttachShader(GL::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_fontshader_vert_glsl"));
	_shader.Link();
	_shader["textBuffer"] = 0;

	_triangle_buffer.SetDataEmpty();
	_vao.SetVertexAttributes<vec3>(_triangle_buffer);

	GLfloat triangles[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	_resolve_vbo.SetData(triangles, sizeof(triangles));
	_resolve_vao.SetVertexAttributes<vec2>(_resolve_vbo);

	_text_buffer = std::make_unique<GL::Framebuffer>(_width, _height);
	_text_buffer->AttachTexture(GL::InternalFormat::RGBA, GL::Format::RGBA, 0);
	_text_buffer->AttachRenderbuffer(GL::InternalFormat::DEPTH24_STENCIL8, GL::Framebuffer::Attachment::DEPTH_STENCIL);
	_text_buffer->SetDrawBuffers({ 0 });

	_initialized = true;
}

void TextRenderer::_ResizeBuffer(unsigned width, unsigned height) {
	if (_text_buffer) {
		if (_text_buffer->GetViewportWidth() != width || _text_buffer->GetViewportHeight() != height) {
			*_text_buffer = GL::Framebuffer(*_text_buffer, width, height);
		}
	} else {
		_width = width;
		_height = height;
	}
}

int TextRenderer::_DrawChars(Font& font, const Color& color, const wchar_t *text, unsigned length, int x, int y, unsigned size) {
	assert(length <= Font::FONT_CACHE_SIZE);

	_Initialize();

	// get the screen metrics
	auto screen = Engine::GetDisplayConfiguration().resolution;
	float px = float(x) / screen.width * 2.0f - 1.0f;
	float py = float(y) / screen.height * -2.0f + 1.0f;
	float sx = float(size) / screen.width * 2.0f;
	float sy = float(size) / screen.height * 2.0f;

	const auto transform = [&px, &py, &sx, &sy](vec3& v) {
		v.x = v.x * sx + px;
		v.y = v.y * sy + py;
	};

	const auto transform2 = [transform](Character::Triangle& triangle) {
		// transform the triangle by transforming its coordinates
		std::for_each(triangle.begin(), triangle.end(), transform);
	};

	std::vector<Character::Triangle> triangles;
	std::vector<Character::Triangle> bezierCurves;

	// load the characters and populate the triangle and Bézier curve vectors.
	for (unsigned i = 0; i < length; i++) {
		const Character& c = font.LoadCharacter(text[i]);
		std::vector<Character::Triangle> cTriangles = c.Triangles();
		std::vector<Character::Triangle> cBezierCurves = c.BezierCurveTriangles();

		// transform the triangles to the correct position and scale
		std::for_each(cTriangles.begin(), cTriangles.end(), transform2);
		std::for_each(cBezierCurves.begin(), cBezierCurves.end(), transform2);

		triangles.insert(triangles.end(), cTriangles.begin(), cTriangles.end());
		bezierCurves.insert(bezierCurves.end(), cBezierCurves.begin(), cBezierCurves.end());

		x += c.Advance() * size;
		px += c.Advance() * sx;
	}

	// set the text buffer as render target
	GL::State::Push();
	_text_buffer->Clear(GL::Framebuffer::BitField::COLOR);

	// enable the stencil buffer, disable the depth buffer
	GL::State::Enable(GL::Capability::STENCIL_TEST);
	GL::State::Disable(GL::Capability::DEPTH_TEST);

	// for anti-aliasing, enable GL_BLEND
	GL::State::Enable(GL::Capability::BLEND);
	GL::State::SetBlendFunction(GL::BlendFactor::ONE, GL::BlendFactor::ONE);

	float subpixelWidth = 2.0f / (screen.width * 8);
	float subpixelHeight = 2.0f / (screen.height * 8);

	_shader["color"] = vec4 { color.r, color.g, color.b, color.a / 4.0f };

	// find the bounds of the final quad
	constexpr float fmin = std::numeric_limits<float>::lowest();
	constexpr float fmax = std::numeric_limits<float>::max();
	vec4 bounds { fmax, fmax, fmin, fmin };

	for (const auto& t : bezierCurves) {
		for (int i = 0; i < 3; i++) {
			bounds[0] = std::min(bounds[0], t[i].x);
			bounds[1] = std::min(bounds[1], t[i].y);
			bounds[2] = std::max(bounds[2], t[i].x);
			bounds[3] = std::max(bounds[3], t[i].y);
		}
	}

	bounds[0] -= 4.0f / screen.width;
	bounds[1] -= 4.0f / screen.width;
	bounds[2] += 4.0f / screen.width;
	bounds[3] += 4.0f / screen.width;

	_shader["bounds"] = bounds;

	_DrawTriangles(triangles, bezierCurves, bounds, { subpixelWidth * -1, subpixelHeight *  3 });
	_DrawTriangles(triangles, bezierCurves, bounds, { subpixelWidth *  3, subpixelHeight *  1 });
	_DrawTriangles(triangles, bezierCurves, bounds, { subpixelWidth * -3, subpixelHeight * -1 });
	_DrawTriangles(triangles, bezierCurves, bounds, { subpixelWidth *  1, subpixelHeight * -3 });

	// reset the gl state
	GL::State::Pop();

	// draw the text to the current framebuffer
	_shader["stage"] = STAGE_COPY;
	_text_buffer->GetTextureAttachment(0).Bind(0);

	_resolve_vao.DrawArrays(GL::VertexArray::Mode::TRIANGLES, 0, 6);

	return x;
}

void TextRenderer::_DrawTriangles(const std::vector<Character::Triangle>& triangles,
		const std::vector<Character::Triangle>& bezierCurves, vec4 bounds,
		vec2 multisampleOffset) {

	// only draw on the stencil buffer
	GL::State::Push();
	GL::State::SetColorMask(false, false, false, false);
	GL::State::SetDepthMask(false);

	// initialize the stencil buffer
	_text_buffer->Clear(GL::Framebuffer::BitField::STENCIL);
	GL::State::SetStencilFunc(GL::CompareFunction::NEVER, 0x01, 0xff);
	GL::State::SetStencilMask(0x01);
	GL::State::SetStencilOp(GL::StencilFunction::INVERT, GL::StencilFunction::INVERT, GL::StencilFunction::INVERT);

	// draw the simple triangles
	_shader["stage"] = STAGE_TRIANGLES;
	_shader["multisampleOffset"] = multisampleOffset;
	_vao.Bind();
	_triangle_buffer.SetData(triangles);

	_vao.DrawArrays(GL::VertexArray::Mode::TRIANGLES, 0, 3 * triangles.size());

	// flip the bezier curves
	_shader["stage"] = STAGE_BEZIER;
	_triangle_buffer.SetData(bezierCurves);

	_vao.DrawArrays(GL::VertexArray::Mode::TRIANGLES, 0, 3 * bezierCurves.size());

	// restore color and depth mask
	GL::State::Pop();

	// setup the resolve stage
	_shader["stage"] = STAGE_RESOLVE;
	GL::State::SetStencilFunc(GL::CompareFunction::EQUAL, 0x01, 0xFF);
	GL::State::SetStencilOp(GL::StencilFunction::KEEP, GL::StencilFunction::KEEP, GL::StencilFunction::KEEP);

	// resolve the stencil buffer
	_resolve_vao.DrawArrays(GL::VertexArray::Mode::TRIANGLES, 0, 6);
}

}
