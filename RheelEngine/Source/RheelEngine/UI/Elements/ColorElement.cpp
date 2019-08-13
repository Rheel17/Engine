#include "ColorElement.h"

#include <iostream>

namespace rheel {

ColorElement::ColorElement(Color color) :
		_color(std::move(color)) {

	SetFocusable(true);
}

ColorElement::ColorElement(unsigned width, unsigned height, Color color) :
		_color(color) {

	SetDefaultSize(width, height);
	SetFocusable(true);
}

void ColorElement::Draw() const {
	_DrawColoredQuad(GetBounds(), _color);

	if (HasFocus()) {
		_DrawColoredQuad({ GetBounds().x, GetBounds().y, GetBounds().width, 5 }, { 0, 0, 0, 1 });
		_DrawColoredQuad({ GetBounds().x, GetBounds().y + GetBounds().height - 5, GetBounds().width, 5 }, { 0, 0, 0, 1 });
		_DrawColoredQuad({ GetBounds().x, GetBounds().y, 5, GetBounds().height }, { 0, 0, 0, 1 });
		_DrawColoredQuad({ GetBounds().x + GetBounds().width - 5, GetBounds().y, 5, GetBounds().height }, { 0, 0, 0, 1 });
	}
}

}
