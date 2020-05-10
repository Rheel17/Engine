/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_FONT_H
#define RHEELENGINE_FONT_H
#include "../../_common.h"

#include <list>

#include "Glyph.h"
#include "../../Util/Cache.h"

namespace rheel {

class RE_API Font {
	RE_NO_COPY(Font)
	RE_NO_MOVE(Font)

private:
	struct delete_free_type_library {
		void operator()(FT_Library *ft) {
			FT_Done_FreeType(*ft);
			delete ft;
		};
	};

public:
	explicit Font(FT_Face face);
	~Font();

	Glyph LoadCharacter(char32_t c);

	unsigned Ascend(unsigned size) const;
	unsigned Descend(unsigned size) const;

	/**
	 * Returns the character width in pixels, using a character and its font
	 * size.
	 */
	unsigned CharacterWidth(char character, unsigned size) const;

	/**
	 * Returns the character width in pixels of the given unicode code point and
	 * its font size.
	 */
	unsigned CharacterWidth(char32_t code, unsigned size) const;

	/**
	 * Returns the string width in pixels of the given unicode string. The
	 * string must be UTF-8 encoded and null-terminated.
	 */
	unsigned StringWidth(const char* str, unsigned size) const;

	/**
	 * Returns the string width in pixels of the given unicode string. The
	 * string must be UTF-8 encoded.
	 * @return
	 */
	unsigned StringWidth(const std::string& str, unsigned size) const;

private:
	FT_Face _face;

public:
	static constexpr auto DEFAULT_FONT = "__default_font__";

	static void Initialize();
	static void RegisterFont(const std::string& filename, const std::string& name);
	static Font& GetFont(const std::string& name);
	static Font& GetDefaultFont();

private:
	static void InitializeFreeType_();

	static std::unique_ptr<FT_Library, delete_free_type_library> _ft;
	static std::unordered_map<std::string, Font> _registered_fonts;

};

}

#endif
