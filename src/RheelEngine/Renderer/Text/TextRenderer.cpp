/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextRenderer.h"

#include "Encoding.h"

namespace rheel {

void TextRenderer::DrawText(const std::string& text, int x, int y, Font& font, unsigned size, const Color& color) {
	DrawText(text.c_str(), x, y, font, size, color);
}

void TextRenderer::DrawText(const char* text, int x, int y, Font& font, unsigned size, const Color& color) {
	const char32_t* chars = GetUnicodeArray_(text);

	while (*chars != 0) {
		x = DrawChars_(&chars, 0, x, y, font, size, color);
	}
}

void TextRenderer::DrawParagraph(
		const std::string& text,
		int x, int y, unsigned width,
		Font& font, unsigned size, const Color& color,
		TextRenderer::TextAlign align) {

	DrawParagraph(text.c_str(), x, y, width, font, size, color, align);
}

void TextRenderer::DrawParagraph(
		const char* text,
		int x, int y, unsigned width,
		Font& font, unsigned size, const Color& color,
		TextRenderer::TextAlign align) {

	const char32_t* chars = GetUnicodeArray_(text);

	unsigned spaceWidth = font.CharacterWidth(U' ') * size;
	unsigned lineHeight = size * 1.1;

	unsigned currentWidth = 0;
	size_t currentOffset = 0;

	unsigned wordStartWidth = 0;
	size_t wordStart = 0;

	const auto drawLine = [&]() {
		if (currentOffset == 0) {
			return;
		}

		size_t newOffset = currentOffset - wordStart;
		unsigned newWidth = currentWidth - wordStartWidth;

		chars -= currentOffset;
		currentOffset = wordStart;
		currentWidth = wordStartWidth;

		DrawParagraphLine_(chars, currentOffset, x, y, width, font, size, color, align, currentWidth);

		chars += currentOffset + newOffset;
		currentOffset = newOffset;
		currentWidth = newWidth;
		wordStart = 0;
		y += lineHeight;
	};

	while (*chars != 0) {
		char32_t c = *(chars++);

		if (c == U'\n') {
			drawLine();
			continue;
		}

		if (c == U' ' || c == U'\t') {
			currentWidth += spaceWidth;
			currentOffset++;
			wordStart = currentOffset;
			wordStartWidth = currentWidth;
			continue;
		}

		if (0 <= c && c < 128 && std::isspace(c)) {
			// TODO: other whitespace handling
			continue;
		}

		unsigned cwidth = font.CharacterWidth(c) * size;
		currentWidth += cwidth;
		currentOffset++;

		if (currentWidth > width && wordStart != 0) {
			drawLine();
		}
	}

	wordStart = currentOffset;
	drawLine();
}

const char32_t* TextRenderer::GetUnicodeArray_(const char* text) {
	_unicode_array.clear();

	while (*text) {
		_unicode_array.push_back(Encoding::ReadUtf8(&text));
	}

	_unicode_array.push_back(U'\0');
	return _unicode_array.data();
}

void TextRenderer::DrawParagraphLine_(
		const char32_t* text, size_t count,
		int x, int y, unsigned int width,
		Font& font, unsigned int size, const Color& color,
		TextRenderer::TextAlign align, unsigned textWidth) {

	switch (align) {
		case TextAlign::LEFT:
			while (*text != 0) {
				const char32_t* prevtext = text;
				x = DrawChars_(&text, count, x, y, font, size, color);
				count -= text - prevtext;
			}
			break;
		case TextAlign::CENTER:
			break;
		case TextAlign::RIGHT:
			break;
	}
}

int TextRenderer::DrawChars_(const char32_t** text, size_t count, int x, int y, Font& font, unsigned size, const Color& color) {
	FontRenderer& renderer = *_renderers.Get(&font, [](Font* font) {
		return std::make_unique<FontRenderer>(*font);
	});

	renderer.SetSize(size);
	renderer.SetColor(color);

	return renderer.Render(text, count, x, y);
}

}
