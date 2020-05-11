/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_TEXTRENDERER_H
#define RHEELENGINE_TEXTRENDERER_H
#include "../../_common.h"

#include "FontRenderer.h"

namespace rheel {

class RE_API TextRenderer {

public:
	/**
	 * Draws the text with the given parameters. Note that the text must be an
	 * UTF-8 encoded string.
	 */
	void DrawText(Font& font, const Color& color, const std::string& text, int x, int y, unsigned size);

	/**
	 * Draws the text with the given parameters. Note that the text must be a
	 * null-terminated UTF-8 encoded string.
	 */
	void DrawText(Font& font, const Color& color, const char* text, int x, int y, unsigned size);

private:
	int DrawChars_(Font& font, const Color& color, const char** text, int x, int y, unsigned size);

	Cache<Font*, std::unique_ptr<FontRenderer>> _renderers;

};

}

#endif
