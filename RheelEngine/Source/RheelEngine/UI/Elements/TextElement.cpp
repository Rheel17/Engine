#include "TextElement.h"

#include "../../Renderer/Text/TextRenderer.h"

namespace rheel {

TextElement::TextElement(std::string text, unsigned size) :
		TextElement(text, Font::GetDefaultFont(), size) {}

TextElement::TextElement(std::wstring text, unsigned size) :
		TextElement(text, Font::GetDefaultFont(), size) {}

TextElement::TextElement(std::string text, Font& font, unsigned size) :
		_font(font), _size(size) {

	_text = std::move(text);
	_wide = false;

	_font_ascend = _font.Ascend(_size);
	_font_descend = _font.Descend(_size);

	if (_wide) {
		SetDefaultSize(_font.StringWidth(_w_text, _size), _font_ascend + _font_descend);
	} else {
		SetDefaultSize(_font.StringWidth(_text, _size), _font_ascend + _font_descend);
	}
}

TextElement::TextElement(std::wstring text, Font& font, unsigned size) :
		_font(font), _size(size) {

	_w_text = std::move(text);
	_wide = true;

	_font_ascend = _font.Ascend(_size);
	_font_descend = _font.Descend(_size);

	if (_wide) {
		SetDefaultSize(_font.StringWidth(_w_text, _size), _font_ascend + _font_descend);
	} else {
		SetDefaultSize(_font.StringWidth(_text, _size), _font_ascend + _font_descend);
	}
}

void TextElement::Draw(float dt) const {
	const Bounds& bounds = GetBounds();

	unsigned textBounds = _font_ascend + _font_descend;
	unsigned y = bounds.y + (bounds.height - textBounds) / 2 + _font_ascend;

	if (_wide) {
		TextRenderer::DrawText(_font, Color { 1.0f, 1.0f, 1.0f, 1.0f }, _w_text, bounds.x, y, _size);
	} else {
		TextRenderer::DrawText(_font, Color { 1.0f, 1.0f, 1.0f, 1.0f }, _text, bounds.x, y, _size);
	}
}

}
