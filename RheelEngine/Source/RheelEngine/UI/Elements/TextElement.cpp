#include "TextElement.h"

#include "../../Renderer/Text/TextRenderer.h"

namespace rheel {

TextElement::TextElement(unsigned size) :
		TextElement(Font::GetDefaultFont(), size) {}

TextElement::TextElement(Font& font, unsigned size) :
		_font(font), _size(size) {

	_font_ascend = _font.Ascend(_size);
	_font_descend = _font.Descend(_size);
}

std::pair<unsigned, unsigned> TextElement::GetDefaultDimensions() const {
	return std::make_pair(1200, _font_ascend + _font_descend);
}

void TextElement::Draw() const {
	const Bounds& bounds = GetBounds();
	_DrawColoredQuad(bounds, { 1, 0, 0, 1 });

	unsigned textBounds = _font_ascend + _font_descend;
	unsigned y = bounds.y + (bounds.height - textBounds) / 2 + _font_ascend;

	TextRenderer::DrawText(_font, L"Testing the text rendering!", bounds.x, y, _size);
}

}
