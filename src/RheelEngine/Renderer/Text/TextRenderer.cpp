/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextRenderer.h"

namespace rheel {

void TextRenderer::DrawText(const std::string& text, int x, int y, Font& font, unsigned size, const Color& color) {
	DrawText(text.c_str(), x, y, font, size, color);
}

void TextRenderer::DrawText(const char* text, int x, int y, Font& font, unsigned size, const Color& color) {
	while (*text != 0) {
		x = DrawChars_(&text, x, y, font, size, color);
	}
}

void TextRenderer::DrawParagraph(const std::string& text, int x, int y, unsigned width, Font& font, unsigned size, const Color& color, TextRenderer::TextAlign align) {
	DrawParagraph(text.c_str(), x, y, width, font, size, color, align);
}

void TextRenderer::DrawParagraph(const char* text, int x, int y, unsigned width, Font& font, unsigned size, const Color& color, TextRenderer::TextAlign align) {
	// TODO: draw the paragraph
}

int TextRenderer::DrawChars_(const char** text, int x, int y, Font& font, unsigned size, const Color& color) {
	FontRenderer& renderer = *_renderers.Get(&font, [](Font* font) {
		return std::make_unique<FontRenderer>(*font);
	});

	renderer.SetSize(size);
	renderer.SetColor(color);

	return renderer.Render(text, x, y);
}

}
