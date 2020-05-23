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

Font::Font(FT_Face face) {
	_glyph_index.reserve(0x110000);
	_glyphs.reserve(face->num_glyphs);
	_glyph_offsets.reserve(face->num_glyphs);

	std::vector<Glyph::Triangle> triangles;
	std::vector<Glyph::Triangle> beziers;

	for (unsigned glyphIndex = 0; glyphIndex < face->num_glyphs; glyphIndex++) {
		triangles.clear();
		beziers.clear();

		FT_Load_Glyph(face, glyphIndex, FT_LOAD_NO_SCALE);
		_glyphs[glyphIndex] = Glyph(face->glyph, face->units_per_EM, triangles, beziers);
		_glyph_offsets[glyphIndex] = LoadGlyph_(triangles, beziers);
	}

	for (char32_t charcode = 0; charcode < 0x110000; charcode++) {
		_glyph_index[charcode] = FT_Get_Char_Index(face, charcode);
	}

	// TODO: calculate these
	_ascend = 0.0f;
	_descend = 0.0f;
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

float Font::StringWidth(const char* str) const {
	unsigned width = 0;

	while (*str != 0) {
		char32_t c = Encoding::ReadUtf8(&str);
		width += CharacterWidth(c);
	}

	return width;
}

float Font::StringWidth(const std::string& str) const {
	return StringWidth(str.c_str());
}

size_t Font::GetGlyphIndex(char32_t c) const {
	return _glyph_index[c];
}

const Glyph& Font::GetGlyph(size_t glyphIndex) const {
	return _glyphs[glyphIndex];
}

const std::pair<unsigned, unsigned>& Font::GetGlyphOffset(size_t glyph) const {
	return _glyph_offsets[glyph];
}

std::pair<unsigned int, unsigned int> Font::LoadGlyph_(const std::vector<Glyph::Triangle>& triangles, const std::vector<Glyph::Triangle>& beziers) {
	size_t startIndex = _glyph_indices.size();
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

	size_t indexCount = _glyph_indices.size() - startIndex;
	return std::make_pair(startIndex, indexCount);
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
