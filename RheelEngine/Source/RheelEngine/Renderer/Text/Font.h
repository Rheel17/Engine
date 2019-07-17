#ifndef FONT_H_
#define FONT_H_

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <unordered_map>
#include <memory>

namespace rheel {

class Font {

public:
	Font(FT_Face face);
	~Font();

private:
	FT_Face _face;

public:
	static constexpr auto DEFAULT_FONT = "__default_font__";

	static void RegisterFont(const std::string& filename, const std::string& name);
	static const Font& GetFont(const std::string& name);
	static const Font& GetDefaultFont();

private:
	static void _InitializeFreeType();

	static std::unordered_map<std::string, Font> _registered_fonts;
	static std::shared_ptr<FT_Library> _ft;

	static constexpr unsigned BITMAP_SIZE = 1024;
	static constexpr unsigned GLYPH_SIZE = 128;

};

}

#endif
