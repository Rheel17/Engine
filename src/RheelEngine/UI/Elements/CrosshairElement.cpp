/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "CrosshairElement.h"

namespace rheel {

CrosshairElement::CrosshairElement(unsigned size) :
		_size(size) {}

void CrosshairElement::Draw(float time, float dt) const {
	const Bounds& bounds = GetBounds();

	Bounds horizontal = {
			unsigned(bounds.x + (bounds.width - _size) / 2.0f),
			unsigned(bounds.y + (bounds.height - _size / 10.0f) / 2.0f),
			_size, unsigned(_size / 10.0f)
	};
	Bounds vertical{
			unsigned(bounds.x + (bounds.width - _size / 10.0f) / 2.0f),
			unsigned(bounds.y + (bounds.height - _size) / 2.0f),
			unsigned(_size / 10.0f), _size
	};

	DrawColoredQuad(horizontal, { 1.0f, 1.0f, 1.0f, 0.2f });
	DrawColoredQuad(vertical, { 1.0f, 1.0f, 1.0f, 0.2f });
}

bool CrosshairElement::IsOpaque() {
	return false;
}

}
