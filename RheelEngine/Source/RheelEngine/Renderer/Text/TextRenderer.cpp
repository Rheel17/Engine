#include "TextRenderer.h"

namespace rheel {

void TextRenderer::DrawText(Font& font, const std::wstring& text, int x, int y, unsigned size) {
	const wchar_t *chars = text.c_str();
	unsigned length = text.length();

	// draw a maximum of Font::NUM_GLYPHS characters at a time.
	while (length > 0) {
		unsigned charsLength = std::min(length, (unsigned) Font::FONT_CACHE_SIZE);

		x = _DrawChars(font, chars, charsLength, x, y, size);

		length -= charsLength;
		chars += charsLength;
	}
}

void TextRenderer::DrawText(Font& font, const std::string& text, int x, int y, unsigned size) {
	std::wstring wide;
	wide.assign(text.begin(), text.end());

	DrawText(font, wide, x, y, size);
}

int TextRenderer::_DrawChars(Font& font, const wchar_t *text, unsigned length, int x, int y, unsigned size) {
	assert(length <= Font::FONT_CACHE_SIZE);

	for (unsigned i = 0; i < length; i++) {
		font.LoadCharacter(text[i]);
	}

	return 0;
}

}
