#ifndef FONT_H_
#define FONT_H_
#include "../../_common.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <list>

#include "Character.h"
#include "../OpenGL/GLTexture2D.h"

namespace rheel {

class RE_API Font {
	RE_NO_COPY(Font)
	RE_NO_MOVE(Font)

private:
	struct _CharacterCacheItem {
		wchar_t character;
		Character character_data;
	};

public:
	Font(FT_Face face);
	~Font();

	const Character& LoadCharacter(wchar_t c);

	unsigned Ascend(unsigned size) const;
	unsigned Descend(unsigned size) const;

private:
	Character _LoadCharacter(wchar_t c);

	FT_Face _face;
	std::list<_CharacterCacheItem> _character_cache;
	std::map<wchar_t, std::list<_CharacterCacheItem>::iterator> _character_cache_reference;

public:
	static constexpr auto DEFAULT_FONT = "__default_font__";

	static void Initialize();
	static void RegisterFont(const std::string& filename, const std::string& name);
	static Font& GetFont(const std::string& name);
	static Font& GetDefaultFont();

	static constexpr unsigned FONT_CACHE_SIZE = 256;

private:
	static void _InitializeFreeType();

	static std::shared_ptr<FT_Library> _ft;
	static std::unordered_map<std::string, Font> _registered_fonts;

};

}

#endif
