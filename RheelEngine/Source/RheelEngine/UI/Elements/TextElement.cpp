#include "TextElement.h"

namespace rheel {

TextElement::TextElement(unsigned size) :
		_size(size) {}

std::pair<unsigned, unsigned> TextElement::GetDefaultDimensions() const {
	return std::make_pair(200, _size);
}

void TextElement::Draw() const {
	_DrawColoredQuad(GetBounds(), { 1, 1, 1, 1 });
}

}
