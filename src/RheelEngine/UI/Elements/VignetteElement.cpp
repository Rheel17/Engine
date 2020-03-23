/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "VignetteElement.h"

namespace rheel {

bool VignetteElement::IsOpaque() {
	return false;
}

void VignetteElement::Draw(float time, float dt) const {
	_DrawColoredQuad(GetBounds(), { 1.0f, 1.0f, 1.0f, 0.5f });
}

}