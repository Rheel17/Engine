/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextRenderer.h"

#include <limits>

#include "Encoding.h"
#include "../Display/DisplayConfiguration.h"
#include "../OpenGL/Context.h"
#include "../../EngineResources.h"

#define STAGE_TRIANGLES     0
#define STAGE_BEZIER        1
#define STAGE_RESOLVE       2
#define STAGE_COPY          3

namespace rheel {

TextRenderer::ogl_data::ogl_data() :
		triangle_buffer(gl::Buffer::Target::ARRAY),
		resolve_vbo(gl::Buffer::Target::ARRAY),
		text_buffer(1, 1) {

	gl::ContextScope cs;

	shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_fontshader_frag_glsl"));
	shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_fontshader_vert_glsl"));
	shader.Link();
	shader["textBuffer"] = 0;

	triangle_buffer.SetDataEmpty();
	vao.SetVertexAttributes<vec3>(triangle_buffer);

	GLfloat triangles[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	resolve_vbo.SetData(triangles, sizeof(triangles));
	resolve_vao.SetVertexAttributes<vec2>(resolve_vbo);

	text_buffer.AttachTexture(gl::InternalFormat::RGBA, gl::Format::RGBA, 0);
	text_buffer.AttachRenderbuffer(gl::InternalFormat::DEPTH24_STENCIL8, gl::Framebuffer::Attachment::DEPTH_STENCIL);
	text_buffer.SetDrawBuffers({ 0 });
}

// This text rendering system is based on the amazing blog post by Evan Wallace:
// https://medium.com/@evanwallace/easy-scalable-text-rendering-on-the-gpu-c3f4d782c5ac

void TextRenderer::DrawText(Font& font, const Color& color, const std::string& text, int x, int y, unsigned size) const {
	DrawText(font, color, text.c_str(), x, y, size);
}

void TextRenderer::DrawText(Font& font, const Color& color, const char* text, int x, int y, unsigned int size) const {
	while (*text != 0) {
		x = DrawChars_(font, color, &text, x, y, size);
	}
}

void TextRenderer::ResizeBuffer_(unsigned width, unsigned height) const {
	if (_ogl_data->text_buffer.GetViewportWidth() != width || _ogl_data->text_buffer.GetViewportHeight() != height) {
		_ogl_data->text_buffer = gl::Framebuffer(_ogl_data->text_buffer, width, height);
	}
}

int TextRenderer::DrawChars_(Font& font, const Color& color, const char** text, int x, int y, unsigned size) const {
	auto screen = DisplayConfiguration::Get().resolution;
	ResizeBuffer_(screen.x, screen.y);

	// get the metrics
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
	for (unsigned i = 0; i < Font::FONT_CACHE_SIZE; i++) {
		char32_t character = Encoding::Utf8ToCodePoint(*text);
		if (character == 0) {
			break;
		}

		const Character& c = font.LoadCharacter(character);
		std::vector<Character::Triangle> cTriangles = c.Triangles();
		std::vector<Character::Triangle> cBezierCurves = c.BezierCurveTriangles();

		// transform the triangles to the correct position and scale
		std::for_each(cTriangles.begin(), cTriangles.end(), transform2);
		std::for_each(cBezierCurves.begin(), cBezierCurves.end(), transform2);

		triangles.insert(triangles.end(), cTriangles.begin(), cTriangles.end());
		bezierCurves.insert(bezierCurves.end(), cBezierCurves.begin(), cBezierCurves.end());

		x += c.Advance() * size;
		px += c.Advance() * sx;

		*text += Encoding::Utf8Lenght(character);
	}

	// set the text buffer as render target
	{
		gl::ContextScope cs;
		_ogl_data->text_buffer.Clear(gl::Framebuffer::BitField::COLOR);

		// enable the stencil buffer, disable the depth buffer
		gl::Context::Current().Enable(gl::Capability::STENCIL_TEST);
		gl::Context::Current().Disable(gl::Capability::DEPTH_TEST);

		// for anti-aliasing, enable GL_BLEND
		gl::Context::Current().Enable(gl::Capability::BLEND);
		gl::Context::Current().SetBlendFunction(gl::BlendFactor::ONE, gl::BlendFactor::ONE);

		float subpixelWidth = 2.0f / (screen.x * 8);
		float subpixelHeight = 2.0f / (screen.y * 8);

		_ogl_data->shader["color"] = vec4{ color.Red(), color.Green(), color.Blue(), color.Alpha() / 4.0f };

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

		for (const auto& t : triangles) {
			for (int i = 1; i < 3; i++) {
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

		_ogl_data->shader["bounds"] = bounds;

		DrawTriangles_(triangles, bezierCurves, { subpixelWidth * -1.0f, subpixelHeight * 3.0f });
		DrawTriangles_(triangles, bezierCurves, { subpixelWidth * 3.0f, subpixelHeight * 1.0f });
		DrawTriangles_(triangles, bezierCurves, { subpixelWidth * -3.0f, subpixelHeight * -1.0f });
		DrawTriangles_(triangles, bezierCurves, { subpixelWidth * 1.0f, subpixelHeight * -3.0f });
	}

	// draw the text to the current framebuffer
	_ogl_data->shader["stage"] = STAGE_COPY;
	_ogl_data->text_buffer.GetTextureAttachment(0).Bind(0);

	_ogl_data->resolve_vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 6);

	return x;
}

void TextRenderer::DrawTriangles_(const std::vector<Character::Triangle>& triangles,
		const std::vector<Character::Triangle>& bezierCurves, vec2 multisampleOffset) const {

	// only draw on the stencil buffer
	{
		gl::ContextScope cs;
		gl::Context::Current().SetColorMask(false, false, false, false);
		gl::Context::Current().SetDepthMask(false);

		// initialize the stencil buffer
		_ogl_data->text_buffer.Clear(gl::Framebuffer::BitField::STENCIL);
		gl::Context::Current().SetStencilFunc(gl::CompareFunction::NEVER, 0x01, 0xff);
		gl::Context::Current().SetStencilMask(0x01);
		gl::Context::Current().SetStencilOp(gl::StencilFunction::INVERT, gl::StencilFunction::INVERT, gl::StencilFunction::INVERT);

		// draw the simple triangles
		_ogl_data->shader["stage"] = STAGE_TRIANGLES;
		_ogl_data->shader["multisampleOffset"] = multisampleOffset;
		_ogl_data->vao.Bind();
		_ogl_data->triangle_buffer.SetData(triangles);

		_ogl_data->vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 3 * triangles.size());

		// flip the bezier curves
		_ogl_data->shader["stage"] = STAGE_BEZIER;
		_ogl_data->triangle_buffer.SetData(bezierCurves);

		_ogl_data->vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 3 * bezierCurves.size());

		// restore color and depth mask
	}

	// setup the resolve stage
	_ogl_data->shader["stage"] = STAGE_RESOLVE;
	gl::Context::Current().SetStencilFunc(gl::CompareFunction::EQUAL, 0x01, 0xFF);
	gl::Context::Current().SetStencilOp(gl::StencilFunction::KEEP, gl::StencilFunction::KEEP, gl::StencilFunction::KEEP);

	// resolve the stencil buffer
	_ogl_data->resolve_vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 6);
}

}
