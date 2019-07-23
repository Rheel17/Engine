#include "TextElement.h"

#include "../../Renderer/Text/TextRenderer.h"

namespace rheel {

TextElement::TextElement(unsigned size) :
		_size(size) {}

std::pair<unsigned, unsigned> TextElement::GetDefaultDimensions() const {
	return std::make_pair(200, _size);
}

void TextElement::Draw() const {
	const Bounds& bounds = GetBounds();
	TextRenderer::DrawText(Font::GetDefaultFont(), L"Testing the text rendering!", bounds.x, bounds.y, _size);
	abort();
}

}
