/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextRenderer.h"

#include "Encoding.h"

namespace rheel {

void TextRenderer::DrawText(const std::string& text, int x, int y, const Font& font, unsigned size, const Color& color) {
	DrawText(text.c_str(), x, y, font, size, color);
}

void TextRenderer::DrawText(const char* text, int x, int y, const Font& font, unsigned size, const Color& color) {
	DrawParagraph(text, x, y, std::numeric_limits<unsigned>::max(), font, size, color, TextAlign::LEFT);
}

void TextRenderer::DrawParagraph(const std::string& text, int x, int y, unsigned width, const Font& font, unsigned size, const Color& color, TextAlign align) {
	DrawParagraph(text.c_str(), x, y, width, font, size, color, align);
}

void TextRenderer::DrawParagraph(const char* text, int x, int y, unsigned width, const Font& font, unsigned size, const Color& color, TextAlign align) {
	FontRenderer& fontRenderer = FontRenderer::Get_(font);
	fontRenderer.SetSize(size);
	fontRenderer.SetColor(color);

	const char32_t* chars = GetUnicodeArray_(text);
	fontRenderer.Render(chars, x, y);
}

PreparedText TextRenderer::PrepareText(const Prepare& prepare) {
	PreparedText::prepare_text_input input{
		.text = GetUnicodeArray_(prepare._text),
		.width = prepare._width,
		.align = prepare._align,
		.font = prepare._font
	};

	return PreparedText(input);
}

void TextRenderer::DrawPrepared(const PreparedText& text, unsigned size) {
	FontRenderer& fontRenderer = FontRenderer::Get_(text.GetFont());

}

const char32_t* TextRenderer::GetUnicodeArray_(const char* text) {
	_unicode_array.clear();

	while (*text) {
		_unicode_array.push_back(Encoding::ReadUtf8(&text));
	}

	_unicode_array.push_back(U'\0');
	return _unicode_array.data();
}

}
