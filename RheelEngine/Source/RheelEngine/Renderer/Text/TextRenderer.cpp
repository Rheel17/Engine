/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "TextRenderer.h"

#include <limits>

#include "../../Engine.h"
#include "../../EngineResources.h"

#define STAGE_TRIANGLES		0
#define STAGE_BEZIER		1
#define STAGE_RESOLVE		2

namespace rheel {

std::unique_ptr<GLBuffer> TextRenderer::_triangle_buffer(nullptr);
std::unique_ptr<GLVertexArray> TextRenderer::_vao(nullptr);
std::unique_ptr<GLBuffer> TextRenderer::_resolve_vbo(nullptr);
std::unique_ptr<GLVertexArray> TextRenderer::_resolve_vao(nullptr);
GLShaderProgram TextRenderer::_shader;
bool TextRenderer::_initialized(false);

// This text rendering system is based on the amazing blog post by Evan Wallace:
// https://medium.com/@evanwallace/easy-scalable-text-rendering-on-the-gpu-c3f4d782c5ac

void TextRenderer::DrawText(Font& font, const Color& color, const std::wstring& text, int x, int y, unsigned size) {
	const wchar_t *chars = text.c_str();
	unsigned length = text.length();

	// draw a maximum of Font::NUM_GLYPHS characters at a time.
	while (length > 0) {
		unsigned charsLength = std::min(length, (unsigned) Font::FONT_CACHE_SIZE);

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

	_shader.AddShaderFromSource(GLShaderProgram::FRAGMENT, EngineResources::PreprocessShader("Shaders_fontshader_frag_glsl"));
	_shader.AddShaderFromSource(GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_fontshader_vert_glsl"));
	_shader.Link();

	_triangle_buffer = std::make_unique<GLBuffer>(GL::BufferTarget::ARRAY);
	_triangle_buffer->SetData(nullptr, 0);

	_vao = std::make_unique<GLVertexArray>();
	_vao->SetVertexAttributes<vec3>(*_triangle_buffer);

	GLfloat triangles[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	_resolve_vbo = std::make_unique<GLBuffer>(GL::BufferTarget::ARRAY);
	_resolve_vbo->SetData(triangles, sizeof(triangles));

	_resolve_vao = std::make_unique<GLVertexArray>();
	_resolve_vao->SetVertexAttributes<vec2>(*_resolve_vbo);

	_initialized = true;
}

int TextRenderer::_DrawChars(Font& font, const Color& color, const wchar_t *text, unsigned length, int x, int y, unsigned size) {
	assert(length <= Font::FONT_CACHE_SIZE);

	_Initialize();

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

	// enable the stencil buffer
	glEnable(GL_STENCIL_TEST);

	// for anti-aliasing, enable GL_BLEND
	bool isBlend = glIsEnabled(GL_BLEND);
	glEnable(GL_BLEND);

	GLint src, dst;
	glGetIntegerv(GL_BLEND_SRC, &src);
	glGetIntegerv(GL_BLEND_DST, &dst);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

	float subpixelWidth = 2.0f / (screen.width * 8);
	float subpixelHeight = 2.0f / (screen.height * 8);

	_shader["color"] = vec4 { color.r, color.g, color.b, color.a / 4.0f };

	_DrawTriangles(triangles, bezierCurves, { subpixelWidth * -1, subpixelHeight *  3 });
	_DrawTriangles(triangles, bezierCurves, { subpixelWidth *  3, subpixelHeight *  1 });
	_DrawTriangles(triangles, bezierCurves, { subpixelWidth * -3, subpixelHeight * -1 });
	_DrawTriangles(triangles, bezierCurves, { subpixelWidth *  1, subpixelHeight * -3 });

	// reset the gl state
	glBlendFunc(src, dst);

	if (!isBlend) {
		glDisable(GL_BLEND);
	}

	glDisable(GL_STENCIL_TEST);
	return x;
}

void TextRenderer::_DrawTriangles(const std::vector<Character::Triangle>& triangles,
		const std::vector<Character::Triangle>& bezierCurves,
		vec2 multisampleOffset) {

	// only draw on the stencil buffer
	GLboolean colorMask[4];
	GLboolean depthMask;
	glGetBooleanv(GL_COLOR_WRITEMASK, colorMask);
	glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
	glColorMask(false, false, false, false);
	glDepthMask(false);

	// initialize the stencil buffer
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_NEVER, 1, 0xFF);
	glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
	glStencilMask(0x01);

	// draw the simple triangles
	_shader["stage"] = STAGE_TRIANGLES;
	_shader["multisampleOffset"] = multisampleOffset;
	_vao->Bind();
	_triangle_buffer->SetData(triangles);

	glDrawArrays(GL_TRIANGLES, 0, 3 * triangles.size());

	// flip the bezier curves
	_shader["stage"] = STAGE_BEZIER;
	_triangle_buffer->SetData(bezierCurves);

	glDrawArrays(GL_TRIANGLES, 0, 3 * bezierCurves.size());

	// restore color and depth mask
	glColorMask(colorMask[0], colorMask[1], colorMask[2], colorMask[3]);
	glDepthMask(depthMask);

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

	bounds[0] -= std::abs(multisampleOffset.x);
	bounds[1] -= std::abs(multisampleOffset.y);
	bounds[2] += std::abs(multisampleOffset.x);
	bounds[3] += std::abs(multisampleOffset.y);

	// setup the resolve stage
	_shader["stage"] = STAGE_RESOLVE;
	_shader["bounds"] = bounds;
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// resolve the stencil buffer
	_resolve_vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

}
