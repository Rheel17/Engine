#include "Font.h"

namespace rheel {

std::shared_ptr<FT_Library> Font::_ft;
std::unordered_map<std::string, Font> Font::_registered_fonts;

Font::Font(FT_Face face) :
		_face(face), _texture(nullptr) {}

Font::~Font() {
	FT_Done_Face(_face);
}

vec4 Font::LoadCharacter(char c) {

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

const Font& Font::GetFont(const std::string& name) {
	auto iter = _registered_fonts.find(name);
	if (iter == _registered_fonts.end()) {
		throw std::runtime_error("Font '" + name + "' not found. Is it registered?");
	}

	return iter->second;
}

const Font& Font::GetDefaultFont() {
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
