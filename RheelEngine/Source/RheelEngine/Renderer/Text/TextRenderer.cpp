#include "TextRenderer.h"

#include <algorithm>

#include "../../Engine.h"
#include "../../Resources.h"
#include "../ImageTexture.h"

namespace rheel {

TextRenderer::_StaticData TextRenderer::_gl;

TextRenderer::_StaticData::~_StaticData() {
	delete vbo;
	delete eab;
	delete vao;
	delete shader;
}

void TextRenderer::_StaticData::Initialize() {
	if (initialized) {
		return;
	}

	vbo = new GLBuffer(GL::BufferTarget::ARRAY);
	eab = new GLBuffer(GL::BufferTarget::ARRAY);
	vao = new GLVertexArray;
	shader = new GLShaderProgram;

	GLfloat triangles[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	vbo->SetData(triangles, sizeof(triangles));
	eab->SetData(std::vector<Character>());

	vao->SetVertexAttributes<vec2>(*vbo);
	vao->SetVertexAttributes<ivec4, vec4>(*eab, sizeof(Character), true);

	shader->AddShaderFromSource(GLShaderProgram::VERTEX, RESOURCE_AS_STRING(Shaders_fontshader_vert_glsl));
	shader->AddShaderFromSource(GLShaderProgram::FRAGMENT, RESOURCE_AS_STRING(Shaders_fontshader_frag_glsl));
	shader->Link();

	initialized = true;
}

void TextRenderer::DrawText(Font& font, const std::wstring& text, int x, int y, unsigned size) {
	const wchar_t *chars = text.c_str();
	unsigned length = text.length();

	// draw a maximum of Font::NUM_GLYPHS characters at a time.
	while (length > 0) {
		unsigned charsLength = std::min(length, (unsigned) Font::NUM_GLYPHS);

		_DrawChars(font, chars, charsLength, x, y, size);

		length -= charsLength;
		chars += charsLength;
	}
}

void TextRenderer::DrawText(Font& font, const std::string& text, int x, int y, unsigned size) {
	std::wstring wide;
	wide.assign(text.begin(), text.end());

	DrawText(font, wide, x, y, size);
}

void TextRenderer::_DrawChars(Font& font, const wchar_t *text, unsigned length, int x, int y, unsigned size) {
	assert(length <= Font::NUM_GLYPHS);
	std::vector<Character> characters;

	// get the screen dimensions to calculate the corrent postition later.
	auto dimensions = Engine::GetDisplayConfiguration().resolution;

	// loop through the characters and fill the character buffer
	for (unsigned i = 0; i < length; i++) {
		wchar_t c = text[i];

		if (c == 0) {
			// render 0x00 as a space, because the \0 character has a special
			// meaning.
			c = ' ';
		}

		Font::CharacterData data = font.LoadCharacter(c);

		characters.push_back(Character {
			{
				data.texture_location,
				data.texture_size
			},
			{
				((x + (data.offset.x * size)) / dimensions.width) * 2 - 1,
				((y + (data.offset.y * size)) / dimensions.height) * -2 + 1,
				((data.size.x * size) / dimensions.width) * 2,
				((data.size.y * size) / dimensions.height) * 2
			}
		});

		x += data.x_advance * size;
	}

	_gl.Initialize();
	_gl.shader->Use();
	_gl.vao->Bind();
	_gl.eab->SetData(characters);

//	_gl.eab->SetData(std::vector<Character> {
//		Character {
//			{ 0, 0, Font::BITMAP_SIZE, Font::BITMAP_SIZE },
//			{ -1.0f, -1.0f, 2.0f, 2.0f }
//		}
//	});

	// TODO: find out why font texture is empty

	font.BindTexture();
	ImageTexture::Get(Material::UV_TEST_TEXTURE.get()).Bind(0);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, characters.size());
}

}
