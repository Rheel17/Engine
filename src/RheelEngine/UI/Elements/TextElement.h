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
	TextElement(const std::string& text, unsigned size, const Color& color, Font& font = Font::GetDefaultFont());

	TextElement(std::string&& text, unsigned size, const Color& color, Font& font = Font::GetDefaultFont());

	void DoDraw(float time, float dt) const override;

	std::string text;
	Color color;
	unsigned size;

private:
	Font& _font;
	float _font_ascend;
	float _font_descend;

};

}

#endif
