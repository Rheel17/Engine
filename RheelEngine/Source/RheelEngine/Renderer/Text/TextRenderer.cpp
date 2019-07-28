#include "TextRenderer.h"

#include "../../Engine.h"
#include "../../Resources.h"

#define STAGE_TRIANGLES		0
#define STAGE_BEZIER		1
#define STAGE_RESOLVE		2

namespace rheel {

std::shared_ptr<GLBuffer> TextRenderer::_triangle_buffer(nullptr);
std::shared_ptr<GLVertexArray> TextRenderer::_vao(nullptr);
std::shared_ptr<GLBuffer> TextRenderer::_screenquad_vbo(nullptr);
std::shared_ptr<GLVertexArray> TextRenderer::_screenquad_vao(nullptr);
GLShaderProgram TextRenderer::_shader;
bool TextRenderer::_initialized(false);

void TextRenderer::DrawText(Font& font, const std::wstring& text, int x, int y, unsigned size) {
	const wchar_t *chars = text.c_str();
	unsigned length = text.length();

	// draw a maximum of Font::NUM_GLYPHS characters at a time.
	while (length > 0) {
		unsigned charsLength = std::min(length, (unsigned) Font::FONT_CACHE_SIZE);

		x = _DrawChars(font, chars, charsLength, x, y, size);

		length -= charsLength;
		chars += charsLength;
	}
}

void TextRenderer::DrawText(Font& font, const std::string& text, int x, int y, unsigned size) {
	std::wstring wide;
	wide.assign(text.begin(), text.end());

	DrawText(font, wide, x, y, size);
}

void TextRenderer::_Initialize() {
	if (_initialized) {
		return;
	}

	_shader.AddShaderFromSource(GLShaderProgram::FRAGMENT, RESOURCE_AS_STRING(Shaders_fontshader_frag_glsl));
	_shader.AddShaderFromSource(GLShaderProgram::VERTEX, RESOURCE_AS_STRING(Shaders_fontshader_vert_glsl));
	_shader.Link();

	_triangle_buffer = std::make_shared<GLBuffer>(GL::BufferTarget::ARRAY);
	_triangle_buffer->SetData(nullptr, 0);

	_vao = std::make_shared<GLVertexArray>();
	_vao->SetVertexAttributes<vec3>(*_triangle_buffer);

	GLfloat triangles[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
	_screenquad_vbo = std::make_shared<GLBuffer>(GL::BufferTarget::ARRAY);
	_screenquad_vbo->SetData(triangles, sizeof(triangles));

	_screenquad_vao = std::make_shared<GLVertexArray>();
	_screenquad_vao->SetVertexAttributes<vec2>(*_screenquad_vbo);

	_initialized = true;
}

int TextRenderer::_DrawChars(Font& font, const wchar_t *text, unsigned length, int x, int y, unsigned size) {
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

	// enable the stencil buffer, and clear it
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	// only draw on the stencil buffer
	GLboolean colorMask[4];
	GLboolean depthMask;
	glGetBooleanv(GL_COLOR_WRITEMASK, colorMask);
	glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
	glColorMask(false, false, false, false);
	glDepthMask(false);

	// initialize the stencil buffer
	glStencilFunc(GL_NEVER, 1, 0xFF);
	glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
	glStencilMask(0x1);

	// draw the simple triangles
	_shader["stage"] = STAGE_TRIANGLES;
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

	// setup the resolve stage
	_shader["stage"] = STAGE_RESOLVE;
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// resolve the stencil buffer
	_screenquad_vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisable(GL_STENCIL_TEST);
	return x;
}

}
