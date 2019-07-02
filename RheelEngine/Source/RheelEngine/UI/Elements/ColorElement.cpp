#include "ColorElement.h"

#include <iostream>

namespace rheel::ui {

ColorElement::ColorElement(unsigned width, unsigned height, Color&& color) :
		_width(width), _height(height), _color(color) {}

std::pair<unsigned, unsigned> ColorElement::GetDefaultDimensions() const {
	return std::make_pair(_width, _height);
}

void ColorElement::Draw() const {
	_DrawColoredQuad(GetBounds(), _color);
}

}
