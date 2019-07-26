#include "Font.h"

#include <fstream>

namespace rheel {

std::shared_ptr<FT_Library> Font::_ft;
std::unordered_map<std::string, Font> Font::_registered_fonts;

Font::Font(FT_Face face) :
		_face(face) {}

Font::~Font() {
	FT_Done_Face(_face);
}

const Character& Font::LoadCharacter(wchar_t c) {
	auto iter = _character_cache_reference.find(c);

	if (iter == _character_cache_reference.end()) {
		// character needs to be loaded

		if (_character_cache.size() == FONT_CACHE_SIZE) {
			// character cache is full

			_CharacterCacheItem back = _character_cache.back();
			_character_cache.pop_back();
			_character_cache_reference.erase(back.character);
		}

		Character data = _LoadCharacter(c);
		_character_cache.push_front({ c, data });
		_character_cache_reference[c] = _character_cache.begin();
	} else {
		// character was already loaded

		auto ref = iter->second;
		Character data = ref->character_data;

		_character_cache.erase(ref);
		_character_cache.push_front({ c, data });
		_character_cache_reference[c] = _character_cache.begin();
	}

	return _character_cache_reference[c]->character_data;
}

unsigned Font::Ascend(unsigned size) const {
	FT_Set_Pixel_Sizes(_face, 0, size);
	return (unsigned) std::ceil(_face->size->metrics.ascender / 64.0f);
}

unsigned Font::Descend(unsigned size) const {
	FT_Set_Pixel_Sizes(_face, 0, size);
	return (unsigned) std::ceil(_face->size->metrics.descender / -64.0f);
}

Character Font::_LoadCharacter(wchar_t c) {
	std::wcout << "_LoadCharacter(" << c << ")" << std::endl;

	// load the character
	if (FT_Load_Char(_face, c, 0)) {
		throw std::runtime_error("Could not load character '" + std::to_string(c) + "'.");
	}

	return Character(_face->glyph);
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
