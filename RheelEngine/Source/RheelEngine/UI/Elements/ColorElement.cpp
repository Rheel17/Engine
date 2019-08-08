#include "ColorElement.h"

#include <iostream>

namespace rheel {

ColorElement::ColorElement(Color color) :
		_color(std::move(color)) {}

ColorElement::ColorElement(unsigned width, unsigned height, Color color) :
		_color(color) {

	SetDefaultSize(width, height);
}

void ColorElement::Draw() const {
	_DrawColoredQuad(GetBounds(), _color);
}

}
