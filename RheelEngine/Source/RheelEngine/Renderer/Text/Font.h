#ifndef FONT_H_
#define FONT_H_
#include "../../_common.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <list>

#include "../OpenGL/GLTexture2D.h"

namespace rheel {

class RE_API Font {
	RE_NO_COPY(Font)
	RE_NO_MOVE(Font)

public:
	struct CharacterData {
		ivec2 texture_location;
		ivec2 texture_size;
		vec2 offset;
		vec2 size;
		float x_advance;
		float scale;
	};

private:
	struct _CharacterCacheItem {
		wchar_t character;
		CharacterData character_data;
	};

public:
	Font(FT_Face face);
	~Font();

	CharacterData LoadCharacter(wchar_t c);

	unsigned Ascend(unsigned size) const;
	unsigned Descend(unsigned size) const;

	void BindTexture(unsigned textureUnit = 0) const;

private:
	void _Initialize() const;
	CharacterData _LoadCharacter(wchar_t c);

	FT_Face _face;
	mutable GLTexture2D *_texture;
	std::list<_CharacterCacheItem> _character_cache;
	std::map<wchar_t, std::list<_CharacterCacheItem>::iterator> _character_cache_reference;
	unsigned _next_character_x = 0;
	unsigned _next_character_y = 0;

public:
	static constexpr auto DEFAULT_FONT = "__default_font__";

	static void Initialize();
	static void RegisterFont(const std::string& filename, const std::string& name);
	static Font& GetFont(const std::string& name);
	static Font& GetDefaultFont();

private:
	static void _InitializeFreeType();

	static std::shared_ptr<FT_Library> _ft;
	static std::unordered_map<std::string, Font> _registered_fonts;

public:
	static constexpr unsigned BITMAP_SIZE = 2048; // 2048

private:
	static constexpr unsigned _GLYPH_DIVISION = 16; // 16
	static constexpr unsigned _GLYPH_SIZE = BITMAP_SIZE / _GLYPH_DIVISION;

public:
	static constexpr unsigned NUM_GLYPHS = _GLYPH_DIVISION * _GLYPH_DIVISION;

};

}

#endif
