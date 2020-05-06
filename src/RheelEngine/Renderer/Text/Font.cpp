/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Font.h"

#include <fstream>

#include "Encoding.h"

namespace rheel {

std::unique_ptr<FT_Library, Font::delete_free_type_library> Font::_ft;
std::unordered_map<std::string, Font> Font::_registered_fonts;

Font::Font(FT_Face face) :
		_face(face) {}

Font::~Font() {
	FT_Done_Face(_face);
}

const Character& Font::LoadCharacter(char32_t c) {
	return _character_cache.Get(c, [this](char32_t c) {
		return LoadCharacter_(c);
	});
}

unsigned Font::Ascend(unsigned size) const {
	FT_Set_Pixel_Sizes(_face, 0, size);
	return (unsigned) std::ceil(_face->size->metrics.ascender / 64.0f);
}

unsigned Font::Descend(unsigned size) const {
	FT_Set_Pixel_Sizes(_face, 0, size);
	return (unsigned) std::ceil(_face->size->metrics.descender / -64.0f);
}

unsigned Font::CharacterWidth(char character, unsigned size) const {
	return CharacterWidth((char32_t) character, size);
}

unsigned Font::CharacterWidth(char32_t character, unsigned size) const {
	if (!_character_cache.ContainsKey(character)) {
		if (FT_Load_Char(_face, character, FT_LOAD_NO_SCALE | FT_LOAD_ADVANCE_ONLY)) {
			std::string ch = Encoding::CodePointToUtf8(character);

			if (ch.empty()) {
				Log::Error() << "Invalid code point: " << (uint32_t) character << " (0x" << std::hex << (uint32_t) character << ")" << std::endl;
				abort();
			}

			Log::Error() << "Could not load character '" << ch << "'." << std::endl;
			abort();
		}

		return size * (_face->glyph->advance.x / float(_face->units_per_EM));
	}

	return size * _character_cache.Get(character).Advance();
}

unsigned Font::StringWidth(const char* str, unsigned size) const {
	unsigned width = 0;

	while (*str != 0) {
		width += CharacterWidth(*str, size);
		str++;
	}

	return width;
}

unsigned Font::StringWidth(const std::string& str, unsigned size) const {
	return StringWidth(str.c_str(), size);
}

Character Font::LoadCharacter_(char32_t c) {
	// load the character
	if (FT_Load_Char(_face, c, FT_LOAD_NO_SCALE)) {
		std::string ch = Encoding::CodePointToUtf8(c);

		if (ch.empty()) {
			Log::Error() << "Invalid code point: " << (uint32_t) c << " (0x" << std::hex << (uint32_t) c << ")" << std::endl;
			abort();
		}

		Log::Error() << "Could not load character '" << ch << "'." << std::endl;
		abort();
	}

	unsigned short em = _face->units_per_EM;
	return Character(_face->glyph, em);
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
	InitializeFreeType_();

	if (_registered_fonts.find(name) != _registered_fonts.end()) {
		Log::Error() << "Font '" << name << "' already registered" << std::endl;
		abort();
	}

	FT_Face face;
	if (FT_New_Face(*_ft, filename.c_str(), 0, &face)) {
		Log::Error() << "Failed to create font from " << filename << std::endl;
		abort();
	}

	_registered_fonts.emplace(name, face);
}

Font& Font::GetFont(const std::string& name) {
	auto iter = _registered_fonts.find(name);
	if (iter == _registered_fonts.end()) {
		Log::Error() << "Font '" << name << "' not found. Is it registered?" << std::endl;
		abort();
	}

	return iter->second;
}

Font& Font::GetDefaultFont() {
	return GetFont(DEFAULT_FONT);
}

void Font::InitializeFreeType_() {
	if (!_ft) {
		FT_Library* library = new FT_Library;
		_ft = std::unique_ptr<FT_Library, delete_free_type_library>(library);

		if (FT_Init_FreeType(_ft.get())) {
			Log::Error() << "FreeType initialization failed." << std::endl;
			abort();
		}
	}
}

}
