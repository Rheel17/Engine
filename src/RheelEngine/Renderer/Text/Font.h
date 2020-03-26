/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef FONT_H_
#define FONT_H_
#include "../../_common.h"

#include <list>

#include "Character.h"

namespace rheel {

class RE_API Font {
	RE_NO_COPY(Font)
	RE_NO_MOVE(Font)

private:
	struct _CharacterCacheItem {
		wchar_t character{};
		Character character_data;
	};

	struct _DeleteFreeTypeLibrary {
		void operator()(FT_Library *ft) {
			FT_Done_FreeType(*ft);
		};
	};

public:
	explicit Font(FT_Face face);
	~Font();

	const Character& LoadCharacter(wchar_t c);

	unsigned Ascend(unsigned size) const;
	unsigned Descend(unsigned size) const;

	unsigned CharacterWidth(char character, unsigned size) const;
	unsigned CharacterWidth(wchar_t character, unsigned size) const;
	unsigned StringWidth(const char *str, unsigned size) const;
	unsigned StringWidth(const wchar_t *str, unsigned size) const;
	unsigned StringWidth(const std::string& str, unsigned size) const;
	unsigned StringWidth(const std::wstring& str, unsigned size) const;

private:
	Character _LoadCharacter(wchar_t c);

	FT_Face _face;
	std::list<_CharacterCacheItem> _character_cache;
	std::unordered_map<wchar_t, std::list<_CharacterCacheItem>::iterator> _character_cache_reference;

public:
	static constexpr auto DEFAULT_FONT = "__default_font__";

	static void Initialize();
	static void RegisterFont(const std::string& filename, const std::string& name);
	static Font& GetFont(const std::string& name);
	static Font& GetDefaultFont();

	static constexpr unsigned FONT_CACHE_SIZE = 256;

private:
	static void _InitializeFreeType();

	static std::unique_ptr<FT_Library, _DeleteFreeTypeLibrary> _ft;
	static std::unordered_map<std::string, Font> _registered_fonts;

};

}

#endif
