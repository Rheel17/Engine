/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_FONT_H
#define RHEELENGINE_FONT_H
#include "../../_common.h"

#include <list>

#include "Glyph.h"
#include "../../Util/Cache.h"
#include "../OpenGL/VertexArray.h"

namespace rheel {

class RE_API Font {
	RE_NO_COPY(Font);
	RE_NO_MOVE(Font);

	friend class FontRenderer;

private:
	struct delete_free_type_library {
		void operator()(FT_Library *ft) {
			FT_Done_FreeType(*ft);
			delete ft;
		};
	};

public:
	explicit Font(FT_Face face);

	float Ascend() const;
	float Descend() const;

	/**
	 * Returns the character width in pixels of the given unicode code point and
	 * its font size.
	 */
	float CharacterWidth(char32_t code) const;

	/**
	 * Returns the string width in pixels of the given unicode string. The
	 * string must be UTF-8 encoded and null-terminated.
	 */
	float StringWidth(const char* str) const;

	/**
	 * Returns the string width in pixels of the given unicode string. The
	 * string must be UTF-8 encoded.
	 * @return
	 */
	float StringWidth(const std::string& str) const;

	/**
	 * Returns the glyph index for the input character.
	 */
	size_t GetGlyphIndex(char32_t c) const;

	/**
	 * Returns the glyph with the given index. Use GetGlyphIndex(char32_t) to
	 * get the glyph index for a character.
	 */
	const Glyph& GetGlyph(size_t glyphIndex) const;

	/**
	 * Returns the offset and size in the index buffer for the input glyph. Note
	 * that the input is not a character but a glyph, and that a character value
	 * does not necessarily equal the glyph index. Use GetGlyphIndex(char32_t)
	 * to get the glyph index for a character.
	 */
	const std::pair<unsigned, unsigned>& GetGlyphOffset(size_t glyph) const;

private:
	std::pair<unsigned int, unsigned int> LoadGlyph_(const std::vector<Glyph::Triangle>& triangles, const std::vector<Glyph::Triangle>& beziers);

	float _ascend;
	float _descend;

	std::vector<uint32_t> _glyph_index;
	std::vector<Glyph> _glyphs;
	std::vector<std::pair<unsigned, unsigned>> _glyph_offsets;

	std::vector<vec3> _glyph_vertices;
	std::vector<unsigned> _glyph_indices;

public:
	static constexpr auto DEFAULT_FONT = "__default_font__";

	static void Initialize();
	static void RegisterFont(const std::string& filename, const std::string& name);
	static const Font& GetFont(const std::string& name);
	static const Font& GetDefaultFont();

private:
	static void InitializeFreeType_();

	static std::unique_ptr<FT_Library, delete_free_type_library> _ft;
	static std::unordered_map<std::string, Font> _registered_fonts;

};

}

#endif
