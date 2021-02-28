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
	TextElement(std::string text, unsigned size, const Color& color, const Font& font = Font::GetDefaultFont());
	TextElement(std::string text, float width_relative_size, const Color& color, const Font& font = Font::GetDefaultFont());

	void DoDraw(float time, float dt) const override;

	std::string text;
	Color color;
	unsigned size;
	float width_relative_size;
	bool enable_width_relative_size;

private:
	const Font& _font;
	float _font_ascend;
	float _font_descend;

};

}

#endif
