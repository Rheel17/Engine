/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextRenderer.h"

#include "Encoding.h"

namespace rheel {

void TextRenderer::DrawText(std::string_view text, int x, int y, const Font& font, unsigned size, const Color& color) {
	DrawParagraph(text, x, y, std::numeric_limits<unsigned>::max(), font, size, color, TextAlign::LEFT);
}

void TextRenderer::DrawParagraph(std::string_view text, int x, int y, unsigned width, const Font& font, unsigned size, const Color& color, TextAlign align) {
	FontRenderer& font_renderer = FontRenderer::_get(font);
	font_renderer.SetSize(size);
	font_renderer.SetColor(color);

	std::u32string_view chars = _get_unicode_array(text);
	font_renderer.Render(chars, x, y, width, align);
}

std::u32string_view TextRenderer::_get_unicode_array(std::string_view text) {
	_unicode_array.clear();
	char32_t c;

	while ((c = Encoding::ReadUtf8(text)) != 0) {
		_unicode_array.push_back(c);
	}

	_unicode_array.push_back(U'\0');
	return _unicode_array.data();
}

}
