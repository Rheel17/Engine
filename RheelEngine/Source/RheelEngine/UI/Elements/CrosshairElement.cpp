/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "CrosshairElement.h"

namespace rheel {

CrosshairElement::CrosshairElement(unsigned size) :
		_size(size) {}

void CrosshairElement::Draw(float dt) const {
	const Bounds& bounds = GetBounds();

	Bounds horizontal = {
			bounds.x + (bounds.width - _size) / 2.0f,
			bounds.y + (bounds.height - _size / 10.0f) / 2.0f,
			_size, _size / 10.0f };
	Bounds vertical {
			bounds.x + (bounds.width - _size / 10.0f) / 2.0f,
			bounds.y + (bounds.height - _size) / 2.0f,
			_size / 10.0f, _size };


	_DrawColoredQuad(horizontal, { 1.0f, 1.0f, 1.0f, 0.2f });
	_DrawColoredQuad(vertical, { 1.0f, 1.0f, 1.0f, 0.2f });
}

bool CrosshairElement::IsOpaque() {
	return false;
}

}
