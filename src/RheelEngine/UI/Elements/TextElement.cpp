/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextElement.h"

#include "../../Renderer/Text/TextRenderer.h"

namespace rheel {

TextElement::TextElement(const std::string& text, unsigned int size, const Color& color, Font& font) :
		text(text),
		color(color),
		size(size),
		_font(font) {

	_font_ascend = _font.Ascend(size);
	_font_descend = _font.Descend(size);
	SetDefaultSize(_font.StringWidth(text, size), _font_ascend + _font_descend);
}

TextElement::TextElement(std::string&& text, unsigned int size, const Color& color, Font& font) :
		text(std::move(text)),
		color(color),
		size(size),
		_font(font) {

	_font_ascend = _font.Ascend(size) / float(size);
	_font_descend = _font.Descend(size) / float(size);
	SetDefaultSize(_font.StringWidth(text, size), size * (_font_ascend + _font_descend));
}

void TextElement::DoDraw(float time, float dt) const {
	const Bounds& bounds = GetBounds();

	unsigned textBounds = size * (_font_ascend + _font_descend);
	unsigned y = bounds.y + (bounds.height - textBounds) / 2 + (size * _font_ascend);

	GetTextRenderer().DrawText(text, bounds.x, y, _font, size, color);
}

}
