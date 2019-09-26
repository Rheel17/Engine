/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "ColorElement.h"

namespace rheel {

ColorElement::ColorElement(Color color) :
		_color(std::move(color)) {}

ColorElement::ColorElement(unsigned width, unsigned height, Color color) :
		_color(color) {

	SetDefaultSize(width, height);
}

void ColorElement::Draw(float dt) const {
	_DrawColoredQuad(GetBounds(), _color);
}

}
