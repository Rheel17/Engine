#include "TextRenderer.h"

#include <algorithm>

namespace rheel {

void TextRenderer::DrawText(Font& font, const std::wstring& text, unsigned x, unsigned y, unsigned size) {
	std::wcout << "DrawText(" << text << ")" << std::endl;

	const wchar_t *chars = text.c_str();
	unsigned length = text.length();

	// draw a maximum of 256 characters at a time.
	while (length > 0) {
		unsigned charsLength = std::min(length, (unsigned) 256);

		_DrawChars(font, chars, charsLength, x, y, size);

		length -= charsLength;
		chars += charsLength;
	}
}

void TextRenderer::DrawText(Font& font, const std::string& text, unsigned x, unsigned y, unsigned size) {
	std::wstring wide;
	wide.assign(text.begin(), text.end());

	DrawText(font, wide, x, y, size);
}

void TextRenderer::_DrawChars(Font& font, const wchar_t *text, unsigned length, unsigned x, unsigned y, unsigned size) {
	assert(length <= 256);
	std::vector<Font::CharacterData> characters;

	for (unsigned i = 0; i < length; i++) {
		wchar_t c = text[i];

		if (c == 0) {
			// render 0x00 as a space, because that character has a special
			// meaning.
			c = ' ';
		}

		characters.push_back(font.LoadCharacter(c));
	}

	// TODO: draw the characters
}

}
