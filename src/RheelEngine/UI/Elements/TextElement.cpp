/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextElement.h"

#include "../../Renderer/Text/TextRenderer.h"

namespace rheel {

TextElement::TextElement(const std::string& text, unsigned size, const Color& color, const Font& font) :
		text(text),
		color(color),
		size(size),
		_font(font) {

	_font_ascend = _font.Ascend();
	_font_descend = _font.Descend();
	SetDefaultSize(_font.StringWidth(text) * size, size * (_font_ascend + _font_descend));
}

TextElement::TextElement(std::string&& text, unsigned size, const Color& color, const Font& font) :
		text(std::move(text)),
		color(color),
		size(size),
		_font(font) {

	_font_ascend = _font.Ascend();
	_font_descend = _font.Descend();
	SetDefaultSize(_font.StringWidth(text) * size, size * (_font_ascend + _font_descend));
}

void TextElement::DoDraw(float time, float dt) const {
	const Bounds& bounds = GetBounds();

	unsigned text_bounds = size * (_font_ascend + _font_descend);
	unsigned y = bounds.y + (bounds.height - text_bounds) / 2 + (size * _font_ascend);

	GetTextRenderer().DrawText(text, bounds.x, y, _font, size, color);
}

}
