/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_TEXTELEMENT_H
#define RHEELENGINE_TEXTELEMENT_H
#include "../../_common.h"

#include "../Element.h"
#include "../../Renderer/Text/Font.h"
#include "../../Renderer/Text/TextRenderer.h"

namespace rheel {

class RE_API TextElement : public Element {

public:
	/**
	 * Create a text component with text and size. The default font is used.
	 */
	TextElement(std::string text, unsigned size);

	/**
	 * Create a text component with text, font, and size.
	 */
	TextElement(std::string text, Font& font, unsigned size);

	/**
	 * Sets the text displayed by this element
	 */
	void SetText(std::string text);

	void DoDraw(float time, float dt) const override;

private:
	std::string _text;

	Font& _font;

	unsigned _font_ascend;
	unsigned _font_descend;
	unsigned _size;

};

}

#endif
