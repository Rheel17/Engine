/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextRenderer.h"

#include <limits>

#include "../../Engine.h"
#include "../../EngineResources.h"
#include "../OpenGL/State.h"

#define STAGE_TRIANGLES        0
#define STAGE_BEZIER        1
#define STAGE_RESOLVE        2
#define STAGE_COPY          3

namespace rheel {

gl::Buffer TextRenderer::_triangle_buffer(gl::Buffer::Target::ARRAY);
gl::VertexArray TextRenderer::_vao;

gl::Buffer TextRenderer::_resolve_vbo(gl::Buffer::Target::ARRAY);
gl::VertexArray TextRenderer::_resolve_vao;

std::unique_ptr<gl::Framebuffer> TextRenderer::_text_buffer;
gl::Program TextRenderer::_shader;

bool TextRenderer::_initialized(false);
unsigned TextRenderer::_width(1);
unsigned TextRenderer::_height(1);

// This text rendering system is based on the amazing blog post by Evan Wallace:
// https://medium.com/@evanwallace/easy-scalable-text-rendering-on-the-gpu-c3f4d782c5ac

void TextRenderer::DrawText(Font& font, const Color& color, const std::wstring& text, int x, int y, unsigned size) {
	const wchar_t* chars = text.c_str();
	unsigned length = text.length();

	// draw a maximum of Font::NUM_GLYPHS characters at a time.
	while (length > 0) {
		unsigned charsLength = std::min(length, Font::FONT_CACHE_SIZE);

		x = DrawChars_(font, color, chars, charsLength, x, y, size);

		length -= charsLength;
		chars += charsLength;
	}
}

void TextRenderer::DrawText(Font& font, const Color& color, const std::string& text, int x, int y, unsigned size) {
	std::wstring wide;
	wide.assign(text.begin(), text.end());

	DrawText(font, color, wide, x, y, size);
}

void TextRenderer::Initialize_() {
	if (_initialized) {
		return;
	}

	_shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_fontshader_frag_glsl"));
	_shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_fontshader_vert_glsl"));
	_shader.Link();
	_shader["textBuffer"] = 0;

	_triangle_buffer.SetDataEmpty();
	_vao.SetVertexAttributes<vec3>(_triangle_buffer);

	GLfloat triangles[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	_resolve_vbo.SetData(triangles, sizeof(triangles));
	_resolve_vao.SetVertexAttributes<vec2>(_resolve_vbo);

	_text_buffer = std::make_unique<gl::Framebuffer>(_width, _height);
	_text_buffer->AttachTexture(gl::InternalFormat::RGBA, gl::Format::RGBA, 0);
	_text_buffer->AttachRenderbuffer(gl::InternalFormat::DEPTH24_STENCIL8, gl::Framebuffer::Attachment::DEPTH_STENCIL);
	_text_buffer->SetDrawBuffers({ 0 });

	_initialized = true;
}

void TextRenderer::ResizeBuffer_(unsigned width, unsigned height) {
	if (_text_buffer) {
		if (_text_buffer->GetViewportWidth() != width || _text_buffer->GetViewportHeight() != height) {
			*_text_buffer = gl::Framebuffer(*_text_buffer, width, height);
		}
	} else {
		_width = width;
		_height = height;
	}
}

int TextRenderer::DrawChars_(Font& font, const Color& color, const wchar_t* text, unsigned length, int x, int y, unsigned size) {
	assert(length <= Font::FONT_CACHE_SIZE);

	Initialize_();

	// get the screen metrics
	auto screen = Engine::GetDisplayConfiguration().resolution;
	float px = float(x) / screen.x * 2.0f - 1.0f;
	float py = float(y) / screen.y * -2.0f + 1.0f;
	float sx = float(size) / screen.x * 2.0f;
	float sy = float(size) / screen.y * 2.0f;

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
	gl::State::Push();
	_text_buffer->Clear(gl::Framebuffer::BitField::COLOR);

	// enable the stencil buffer, disable the depth buffer
	gl::State::Enable(gl::Capability::STENCIL_TEST);
	gl::State::Disable(gl::Capability::DEPTH_TEST);

	// for anti-aliasing, enable GL_BLEND
	gl::State::Enable(gl::Capability::BLEND);
	gl::State::SetBlendFunction(gl::BlendFactor::ONE, gl::BlendFactor::ONE);

	float subpixelWidth = 2.0f / (screen.x * 8);
	float subpixelHeight = 2.0f / (screen.y * 8);

	_shader["color"] = vec4{ color.r, color.g, color.b, color.a / 4.0f };

	// find the bounds of the final quad
	constexpr float fmin = std::numeric_limits<float>::lowest();
	constexpr float fmax = std::numeric_limits<float>::max();
	vec4 bounds{ fmax, fmax, fmin, fmin };

	for (const auto& t : bezierCurves) {
		for (int i = 0; i < 3; i++) {
			bounds[0] = std::min(bounds[0], t[i].x);
			bounds[1] = std::min(bounds[1], t[i].y);
			bounds[2] = std::max(bounds[2], t[i].x);
			bounds[3] = std::max(bounds[3], t[i].y);
		}
	}

	bounds[0] -= 4.0f / screen.x;
	bounds[1] -= 4.0f / screen.y;
	bounds[2] += 4.0f / screen.x;
	bounds[3] += 4.0f / screen.y;

	_shader["bounds"] = bounds;

	DrawTriangles_(triangles, bezierCurves, bounds, { subpixelWidth * -1, subpixelHeight * 3 });
	DrawTriangles_(triangles, bezierCurves, bounds, { subpixelWidth * 3, subpixelHeight * 1 });
	DrawTriangles_(triangles, bezierCurves, bounds, { subpixelWidth * -3, subpixelHeight * -1 });
	DrawTriangles_(triangles, bezierCurves, bounds, { subpixelWidth * 1, subpixelHeight * -3 });

	// reset the gl state
	gl::State::Pop();

	// draw the text to the current framebuffer
	_shader["stage"] = STAGE_COPY;
	_text_buffer->GetTextureAttachment(0).Bind(0);

	_resolve_vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 6);

	return x;
}

void TextRenderer::DrawTriangles_(const std::vector<Character::Triangle>& triangles,
		const std::vector<Character::Triangle>& bezierCurves,
		vec4 bounds,
		vec2 multisampleOffset) {

	// only draw on the stencil buffer
	gl::State::Push();
	gl::State::SetColorMask(false, false, false, false);
	gl::State::SetDepthMask(false);

	// initialize the stencil buffer
	_text_buffer->Clear(gl::Framebuffer::BitField::STENCIL);
	gl::State::SetStencilFunc(gl::CompareFunction::NEVER, 0x01, 0xff);
	gl::State::SetStencilMask(0x01);
	gl::State::SetStencilOp(gl::StencilFunction::INVERT, gl::StencilFunction::INVERT, gl::StencilFunction::INVERT);

	// draw the simple triangles
	_shader["stage"] = STAGE_TRIANGLES;
	_shader["multisampleOffset"] = multisampleOffset;
	_vao.Bind();
	_triangle_buffer.SetData(triangles);

	_vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 3 * triangles.size());

	// flip the bezier curves
	_shader["stage"] = STAGE_BEZIER;
	_triangle_buffer.SetData(bezierCurves);

	_vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 3 * bezierCurves.size());

	// restore color and depth mask
	gl::State::Pop();

	// setup the resolve stage
	_shader["stage"] = STAGE_RESOLVE;
	gl::State::SetStencilFunc(gl::CompareFunction::EQUAL, 0x01, 0xFF);
	gl::State::SetStencilOp(gl::StencilFunction::KEEP, gl::StencilFunction::KEEP, gl::StencilFunction::KEEP);

	// resolve the stencil buffer
	_resolve_vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 6);
}

}
