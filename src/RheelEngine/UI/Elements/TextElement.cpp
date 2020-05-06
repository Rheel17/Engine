/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextElement.h"

#include "../../Renderer/Text/TextRenderer.h"

namespace rheel {

TextElement::TextElement(std::string text, unsigned size) :
		TextElement(text, Font::GetDefaultFont(), size) {}

TextElement::TextElement(std::string text, Font& font, unsigned size) :
		_font(font),
		_size(size) {

	_text = std::move(text);

	_font_ascend = _font.Ascend(_size);
	_font_descend = _font.Descend(_size);

	SetDefaultSize(_font.StringWidth(_text, _size), _font_ascend + _font_descend);
}

void TextElement::SetText(std::string text) {
	_text = text;
	SetDefaultSize(_font.StringWidth(_text, _size), _font_ascend + _font_descend);
}

void TextElement::DoDraw(float time, float dt) const {
	const Bounds& bounds = GetBounds();

	unsigned textBounds = _font_ascend + _font_descend;
	unsigned y = bounds.y + (bounds.height - textBounds) / 2 + _font_ascend;

	GetTextRenderer().DrawText(_font, Color{ 1.0f, 1.0f, 1.0f, 1.0f }, _text, bounds.x, y, _size);
}

}
