/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_TEXTRENDERER_H
#define RHEELENGINE_TEXTRENDERER_H
#include "../../_common.h"

#include "FontRenderer.h"

namespace rheel {

class RE_API TextRenderer {
	enum class TextAlign {
		LEFT, CENTER, RIGHT
	};

public:
	/**
	 * Draws the text with the given parameters. The text will be drawn in a
	 * single line.
	 */
	void DrawText(const std::string& text, int x, int y, Font& font, unsigned size, const Color& color);

	/**
	 * Draws the text with the given parameters. The text will be drawn in a
	 * single line.
	 */
	void DrawText(const char* text, int x, int y, Font& font, unsigned size, const Color& color);

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
	 */
	void DrawParagraph(const std::string& text, int x, int y, unsigned width, Font& font, unsigned size, const Color& color, TextAlign align);

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
	 */
	void DrawParagraph(const char* text, int x, int y, unsigned width, Font& font, unsigned size, const Color& color, TextAlign align);

private:
	int DrawChars_(const char** text, int x, int y, Font& font, unsigned size, const Color& color);

	Cache<Font*, std::unique_ptr<FontRenderer>> _renderers;
	unsigned _tab_boundary;

};

}

#endif
