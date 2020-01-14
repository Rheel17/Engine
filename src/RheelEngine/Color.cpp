/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Color.h"

#include <tuple>

namespace rheel {

Color::operator vec4() const {
	return { r, g, b, a };
}

bool Color::operator==(const Color& color) const {
	return r == color.r && g == color.g && b == color.b && a == color.a;
}

bool Color::operator<(const Color& color) const {
	return std::tie(r, g, b, a) < std::tie(color.r, color.g, color.b, color.a);
}

}
