#include "TextRenderer.h"

#include "../../Engine.h"
#include "../../Resources.h"

namespace rheel {

std::shared_ptr<GLBuffer> TextRenderer::_triangle_buffer(nullptr);
std::shared_ptr<GLVertexArray> TextRenderer::_vao(nullptr);
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

		break;
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
	_vao->SetVertexAttributes<vec2>(*_triangle_buffer);

	_initialized = true;
}

int TextRenderer::_DrawChars(Font& font, const wchar_t *text, unsigned length, int x, int y, unsigned size) {
	assert(length <= Font::FONT_CACHE_SIZE);

	_Initialize();

//	float scale = size / float(Engine::GetDisplayConfiguration().resolution.height);

//	glEnable(GL_STENCIL_TEST);
//	glClear(GL_STENCIL_BUFFER_BIT);

	_shader.Use();
	_vao->Bind();

	std::vector<Character::Triangle> triangles;

	for (unsigned i = 0; i < length; i++) {
		Character c = font.LoadCharacter(text[i]);
		const std::vector<Character::Triangle>& characterTriangles = c.Triangles();
		triangles.insert(triangles.end(), characterTriangles.begin(), characterTriangles.end());
	}

	// TODO: find out how color and stencil buffers work together, and how
	// to actually render to the stencil buffer in stead of the color
	// buffer.

	_triangle_buffer->SetData(triangles);

	glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);

//	glDisable(GL_STENCIL_TEST);

	return 0;
}

}
