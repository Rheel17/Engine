/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Color.h"

#include <tuple>

namespace rheel {

float clamp(float f) {
	return glm::clamp(f, 0.0f, 1.0f);
}

Color::Color() :
		_r(0.0f),
		_g(0.0f),
		_b(0.0f),
		_a(0.0f) {}

Color::Color(float r, float g, float b, float a) :
		_r(clamp(r)),
		_g(clamp(g)),
		_b(clamp(b)),
		_a(clamp(a)) {}

Color::Color(int r, int g, int b, int a) :
		_r(clamp(r / 255.0f)),
		_g(clamp(g / 255.0f)),
		_b(clamp(b / 255.0f)),
		_a(clamp(a / 255.0f)) {}

Color::Color(unsigned int rgb, int a) :
		_r(((rgb >> 16) & 0xff) / 255.0f),
		_g(((rgb >> 8) & 0xff) / 255.0f),
		_b((rgb & 0xff) / 255.0f),
		_a(clamp(a / 255.0f)) {}

Color::operator vec4() const {
	return { _r, _g, _b, _a };
}

Color::operator ivec4() const {
	return { _r * 255, _g * 255, _b * 255, _a * 255 };
}

bool Color::operator==(const Color& color) const {
	return _r == color._r && _g == color._g && _b == color._b && _a == color._a;
}

bool Color::operator<(const Color& color) const {
	return std::tie(_r, _g, _b, _a) < std::tie(color._r, color._g, color._b, color._a);
}

float Color::Red() const {
	return _r;
}

float Color::Green() const {
	return _g;
}

float Color::Blue() const {
	return _b;
}

float Color::Alpha() const {
	return _a;
}

}
