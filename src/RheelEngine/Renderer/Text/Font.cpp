/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Font.h"

#include <fstream>

#include "Encoding.h"
#include "../../Util/MsTimer.h"

namespace rheel {

std::unique_ptr<FT_Library, Font::delete_free_type_library> Font::_ft;
std::unordered_map<std::string, Font> Font::_registered_fonts;

// TODO: let the user decide before initializing what character range they want,
//  so unicode/ascii/etc. Load only the necessary glyphs.

Font::Font(FT_Face face) :
		_ascend(static_cast<float>(face->ascender) / static_cast<float>(face->units_per_EM)),
		_descend(static_cast<float>(face->descender) / static_cast<float>(face->units_per_EM)) {

	_glyph_index.reserve(0x110000);
	_glyphs.reserve(face->num_glyphs);
	_glyph_offsets.reserve(face->num_glyphs);

	std::vector<Glyph::Triangle> triangles;
	std::vector<Glyph::Triangle> beziers;

	for (unsigned glyph_index = 0; glyph_index < face->num_glyphs; glyph_index++) {
		triangles.clear();
		beziers.clear();

		FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_SCALE);
		_glyphs[glyph_index] = Glyph(face->glyph, face->units_per_EM, triangles, beziers);
		_glyph_offsets[glyph_index] = _load_glyph(triangles, beziers);
	}

	for (char32_t charcode = 0; charcode < 0x110000; charcode++) {
		_glyph_index[charcode] = FT_Get_Char_Index(face, charcode);
	}
}

float Font::Ascend() const {
	return _ascend;
}

float Font::Descend() const {
	return _descend;
}

float Font::CharacterWidth(char32_t character) const {
	return _glyphs[_glyph_index[character]].Advance();
}

float Font::StringWidth(std::string_view str) const {
	unsigned width = 0;
	char32_t c;

	while ((c = Encoding::ReadUtf8(str)) != 0) {
		width += CharacterWidth(c);
	}

	return width;
}

size_t Font::GetGlyphIndex(char32_t c) const {
	return _glyph_index[c];
}

const Glyph& Font::GetGlyph(size_t glyph_index) const {
	return _glyphs[glyph_index];
}

const std::pair<unsigned, unsigned>& Font::GetGlyphOffset(size_t glyph) const {
	return _glyph_offsets[glyph];
}

std::pair<unsigned int, unsigned int> Font::_load_glyph(const std::vector<Glyph::Triangle>& triangles, const std::vector<Glyph::Triangle>& beziers) {
	size_t start_index = _glyph_indices.size();
	size_t offset = _glyph_vertices.size();

	// common point for the triangles
	if (triangles.size() > 0) {
		_glyph_vertices.push_back(triangles[0][0]);
	}

	// other points for triangles
	for (size_t i = 0; i < triangles.size(); i++) {
		_glyph_vertices.push_back(triangles[i][1]);
		_glyph_vertices.push_back(triangles[i][2]);

		_glyph_indices.push_back(offset);
		_glyph_indices.push_back(offset + i * 2 + 1);
		_glyph_indices.push_back(offset + i * 2 + 2);
	}

	offset = _glyph_vertices.size();

	// bezier curves
	for (const auto& triangle : beziers) {
		_glyph_vertices.push_back(triangle[0]);
		_glyph_vertices.push_back(triangle[1]);
		_glyph_vertices.push_back(triangle[2]);

		_glyph_indices.push_back(offset++);
		_glyph_indices.push_back(offset++);
		_glyph_indices.push_back(offset++);
	}

	size_t index_count = _glyph_indices.size() - start_index;
	return std::make_pair(start_index, index_count);
}

void Font::Initialize() {
	std::vector<std::string> test_defaults = {
#if defined(_WIN32)
			"C:/Windows/Fonts/ARIALUNI.TTF",
			"C:/Windows/Fonts/arial.ttf"
#elif defined(__linux__)
			"/usr/share/fonts/truetype/freefont/FreeSans.ttf"
#endif
	};

	for (const auto& test : test_defaults) {
		if (std::ifstream(test).good()) {
			RegisterFont(test, default_font);
			break;
		}
	}
}

void Font::RegisterFont(const std::string& filename, const std::string& name) {
	_initialize_free_type();

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
	FT_Done_Face(face);
}

const Font& Font::GetFont(const std::string& name) {
	auto iter = _registered_fonts.find(name);
	if (iter == _registered_fonts.end()) {
		Log::Error() << "Font '" << name << "' not found. Is it registered?" << std::endl;
		abort();
	}

	return iter->second;
}

const Font& Font::GetDefaultFont() {
	return GetFont(default_font);
}

void Font::_initialize_free_type() {
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
