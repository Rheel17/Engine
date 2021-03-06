/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ColorElement.h"

namespace rheel {

ColorElement::ColorElement(Color color) :
		color(color) {}

ColorElement::ColorElement(unsigned width, unsigned height, Color color) :
		color(color) {

	SetDefaultSize(width, height);
}

void ColorElement::DoDraw(float time, float dt) const {
	DrawColoredQuad(GetBounds(), color);
}

}
