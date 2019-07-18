#ifndef FONT_H_
#define FONT_H_

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <unordered_map>
#include <memory>

#include "../OpenGL/GLTexture2D.h"

namespace rheel {

class Font {

	// disable copy and move
	Font(const Font&) = delete;
	Font(Font&&) = delete;
	Font& operator=(const Font&) = delete;
	Font& operator=(Font&&) = delete;

public:
	Font(FT_Face face);
	~Font();

	vec4 LoadCharacter(char c);

private:
	FT_Face _face;
	GLTexture2D *_texture;

public:
	static constexpr auto DEFAULT_FONT = "__default_font__";

	static void RegisterFont(const std::string& filename, const std::string& name);
	static const Font& GetFont(const std::string& name);
	static const Font& GetDefaultFont();

private:
	static void _InitializeFreeType();

	static std::shared_ptr<FT_Library> _ft;
	static std::unordered_map<std::string, Font> _registered_fonts;

	static constexpr unsigned BITMAP_SIZE = 2048;
	static constexpr unsigned GLYPH_SIZE = 128;

};

}

#endif
