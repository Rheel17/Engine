/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextRenderer.h"

namespace rheel {

void TextRenderer::DrawText(Font& font, const Color& color, const std::string& text, int x, int y, unsigned size) {
	DrawText(font, color, text.c_str(), x, y, size);
}

void TextRenderer::DrawText(Font& font, const Color& color, const char* text, int x, int y, unsigned int size) {
	while (*text != 0) {
		x = DrawChars_(font, color, &text, x, y, size);
	}
}

int TextRenderer::DrawChars_(Font& font, const Color& color, const char** text, int x, int y, unsigned size) {
	FontRenderer& renderer = *_renderers.Get(&font, [](Font* font) {
		return std::make_unique<FontRenderer>(*font);
	});

	renderer.SetSize(size);
	renderer.SetColor(color);

	return renderer.Render(text, x, y);
}

}
