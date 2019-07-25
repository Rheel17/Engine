#include "Font.h"

#include <fstream>

#include "../../Image.h"
#include "GlyphDistanceField.h"

#include <string_format.h>

namespace rheel {

std::shared_ptr<FT_Library> Font::_ft;
std::unordered_map<std::string, Font> Font::_registered_fonts;

Font::Font(FT_Face face) :
		_face(face), _texture(nullptr) {}

Font::~Font() {
	FT_Done_Face(_face);
	delete _texture;
}

Font::CharacterData Font::LoadCharacter(wchar_t c) {
	_Initialize();

	auto iter = _character_cache_reference.find(c);

	if (iter == _character_cache_reference.end()) {
		// character needs to be loaded

		if (_character_cache.size() == NUM_GLYPHS) {
			// character cache is full

			_CharacterCacheItem back = _character_cache.back();
			_character_cache.pop_back();
			_character_cache_reference.erase(back.character);

			_next_character_x = back.character_data.texture_location.x;
			_next_character_y = back.character_data.texture_location.y;
		}

		CharacterData data = _LoadCharacter(c);
		_character_cache.push_front({ c, data });
		_character_cache_reference[c] = _character_cache.begin();

		return data;
	} else {
		// character was already loaded

		auto ref = iter->second;
		CharacterData data = ref->character_data;

		_character_cache.erase(ref);
		_character_cache.push_front({ c, data });
		_character_cache_reference[c] = _character_cache.begin();

		return data;
	}
}

unsigned Font::Ascend(unsigned size) const {
	FT_Set_Pixel_Sizes(_face, 0, size);
	return (unsigned) std::ceil(_face->size->metrics.ascender / 64.0f);
}

unsigned Font::Descend(unsigned size) const {
	FT_Set_Pixel_Sizes(_face, 0, size);
	return (unsigned) std::ceil(_face->size->metrics.descender / -64.0f);
}

void Font::BindTexture(unsigned textureUnit) const {
	_Initialize();
	_texture->Bind(textureUnit);
}

void Font::_Initialize() const {
	if (!_texture) {
		_texture = new GLTexture2D(BITMAP_SIZE, BITMAP_SIZE, GL_R32F);
		_texture->SetMinifyingFilter(GL::FilterFunction::LINEAR);
		_texture->SetMagnificationFilter(GL::FilterFunction::LINEAR);
		_texture->SetWrapParameterS(GL::WrapParameter::CLAMP_TO_EDGE);
		_texture->SetWrapParameterT(GL::WrapParameter::CLAMP_TO_EDGE);

		// initialize the data
		_texture->InitializeEmpty();
	}
}

Font::CharacterData Font::_LoadCharacter(wchar_t c) {
	// load the character
	FT_Set_Pixel_Sizes(_face, 0, _GLYPH_SIZE - 2 * GlyphDistanceField::PADDING);
	if (FT_Load_Char(_face, c, FT_LOAD_RENDER)) {
		throw std::runtime_error("Could not load character '" + std::to_string(c) + "'.");
	}

	unsigned width = _face->glyph->bitmap.width;
	unsigned height = _face->glyph->bitmap.rows;
	unsigned textureWidth = width;
	unsigned textureHeight = height;
	float scale = 1.0f;

	if (width > 0 && height > 0) {
		// create the signed distance field
		GlyphDistanceField distanceField(_face->glyph->bitmap.buffer, width, height);
		textureWidth += 2 * GlyphDistanceField::PADDING;
		textureHeight += 2 * GlyphDistanceField::PADDING;

		// copy / scale the texture
		if (textureWidth <= _GLYPH_SIZE && textureHeight <= _GLYPH_SIZE) {
			// copy the glyph to the texture
			if (width > 0 && height > 0) {
				_texture->SetPartialData(
						_GLYPH_SIZE * _next_character_x,
						_GLYPH_SIZE * _next_character_y,
						textureWidth, textureHeight,
						GL_RGBA,
						GL_FLOAT,
						distanceField.Data());
			}
		} else {
			// The glyph is too big. We eventually want to scale here.
			// TODO scale the glyph, and upload to the gpu.
			// TODO Maybe: redraw the glyph with a lower size?
			// TODO Maybe: large glyphs take multiple slots?

			throw std::runtime_error("Glyph has exceeded max size. Character not supported.");
		}
	}

	// create the return data
	CharacterData data = {
			// texture location
			{ _next_character_x * _GLYPH_SIZE, _next_character_y * _GLYPH_SIZE },

			// texture size
			{ textureWidth, textureHeight },

			// offset
			{
					_face->glyph->bitmap_left / float(_GLYPH_SIZE),
					((int) height - (int) _face->glyph->bitmap_top) / float(_GLYPH_SIZE)
			},

			// size
			{
					width / float(_GLYPH_SIZE),
					height / float(_GLYPH_SIZE)
			},

			// advance
			(_face->glyph->advance.x / 64.0f) / float(_GLYPH_SIZE),

			// scale
			scale
	};

	// advance the next character position
	_next_character_x++;
	_next_character_x %= _GLYPH_DIVISION;

	if (_next_character_x == 0) {
		_next_character_y++;
	}

//	float *loadData = new float[BITMAP_SIZE * BITMAP_SIZE * 4];
//	_texture->Bind();
//	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, loadData);
//
//	unsigned index = 0;
//	for (unsigned i = 0; i < BITMAP_SIZE; i++) {
//		for (unsigned j = 0; j < BITMAP_SIZE; j++) {
//			if (loadData[index] > 0.0f) {
//				std::cout << "x";
//			} else {
//				std::cout << ".";
//			}
//			index += 4;
//		}
//
//		std::cout << std::endl;
//	}
//
//	std::cout << std::endl;
//	std::cout << std::endl;
//
//	delete[] loadData;

	// return the data
	return data;
}

void Font::Initialize() {
	std::vector<std::string> testDefaults = {
			"C:/Windows/Fonts/ARIALUNI.TTF",
			"C:/Windows/Fonts/arial.ttf"
	};

	for (auto test : testDefaults) {
		if (std::ifstream(test).good()) {
			RegisterFont(test, DEFAULT_FONT);
			break;
		}
	}
}

void Font::RegisterFont(const std::string& filename, const std::string& name) {
	_InitializeFreeType();

	if (_registered_fonts.find(name) != _registered_fonts.end()) {
		throw std::runtime_error("Font '" + name + "' already registered");
	}

	FT_Face face;
	if (FT_New_Face(*_ft, filename.c_str(), 0, &face)) {
		throw std::runtime_error("Failed to create font from " + filename);
	}

	_registered_fonts.emplace(name, face);
}

Font& Font::GetFont(const std::string& name) {
	auto iter = _registered_fonts.find(name);
	if (iter == _registered_fonts.end()) {
		throw std::runtime_error("Font '" + name + "' not found. Is it registered?");
	}

	return iter->second;
}

Font& Font::GetDefaultFont() {
	return GetFont(DEFAULT_FONT);
}

void Font::_InitializeFreeType() {
	if (!_ft) {
		FT_Library *library = new FT_Library;
		_ft = std::shared_ptr<FT_Library>(library, [](FT_Library *ft) {
			FT_Done_FreeType(*ft);
		});

		if (FT_Init_FreeType(_ft.get())) {
			throw std::runtime_error("FreeType initialization failed.");
		}
	}
}

}
