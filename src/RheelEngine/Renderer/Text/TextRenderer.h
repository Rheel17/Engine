/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_TEXTRENDERER_H
#define RHEELENGINE_TEXTRENDERER_H
#include "../../_common.h"

#include "FontRenderer.h"
#include "PreparedText.h"

namespace rheel {

class RE_API TextRenderer {

public:
	/**
	 * Draws the text with the given parameters. The text will be drawn in a
	 * single line.
	 */
	void DrawText(const std::string& text, int x, int y, const Font& font, unsigned size, const Color& color);

	/**
	 * Draws the text with the given parameters. The text will be drawn in a
	 * single line.
	 */
	void DrawText(const char* text, int x, int y, const Font& font, unsigned size, const Color& color);

	/**
	 * Draws the text as a paragraph. Newline characters ('\n', u+000a) will be
	 * interpreted as line break. Words will not be split. Any whitespace will
	 * be counted as a word boundary, and can trigger a new line to make sure
	 * the paragraph fits within the specified width. Words longer than the
	 * width will not be split and will be rendered past the paragraph bounds.
	 *
	 * Tabs will be rendered such that the first character after the tab will
	 * begin at the next 4-space boundary. Use SetTabBoundary() to configure
	 * this. Except for spaces and tabs, all whitespace characters will have a
	 * width of zero.
	 *
	 * The text will be rendered between x and x+width horizontally, with the
	 * first line's baseline as y.
	 */
	void DrawParagraph(const std::string& text, int x, int y, unsigned width, const Font& font, unsigned size, const Color& color, TextAlign align);

	/**
	 * Draws the text as a paragraph. Newline characters ('\n', u+000a) will be
	 * interpreted as line break. Words will not be split. Any whitespace will
	 * be counted as a word boundary, and can trigger a new line to make sure
	 * the paragraph fits within the specified width. Words longer than the
	 * width will not be split and will be rendered past the paragraph bounds.
	 *
	 * Tabs will be rendered such that the first character after the tab will
	 * begin at the next 4-space boundary. Use SetTabBoundary() to configure
	 * this. Except for spaces and tabs, all whitespace characters will have a
	 * width of zero.
	 *
	 * The text will be rendered between x and x+width horizontally, with the
	 * first line's baseline as y.
	 */
	void DrawParagraph(const char* text, int x, int y, unsigned width, const Font& font, unsigned size, const Color& color, TextAlign align);

private:
	// pointer will be freed when the textrenderer gets destructed. This is to
	// re-use its memory. Under the hood, it uses a std::vector.
	const char32_t* GetUnicodeArray_(const char* text);

	std::vector<char32_t> _unicode_array;

};

}

#endif
