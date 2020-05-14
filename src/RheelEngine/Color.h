/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_COLOR_H
#define RHEELENGINE_COLOR_H
#include "_common.h"

namespace rheel {

class RE_API Color {

public:
	/**
	 * The default color: r = g = b = a = 0.
	 */
	Color();

	/**
	 * Constructs a color with floating point red, green, blue, alpha
	 * components. Each component will be clamped to the interval [0 .. 1].
	 */
	Color(float r, float g, float b, float a = 1.0f);

	/**
	 * Constructs a color with integer red, green, blue, alpha components. Each
	 * component will be clamped to the interval [0 .. 255].
	 */
	Color(int r, int g, int b, int a = 255);

	/**
	 * Constructs a color from a rgb integer, and optional alpha. The alpha will
	 * be clamped to the interval [0 .. 255]. The rgb integer will be parsed as
	 * 0x00rrggbb.
	 */
	Color(unsigned rgb, int a = 255);

	/**
	 * Casts the color to a floating point 4-component vector, returned as
	 * [ r, g, b, a ], with each component in the interval [0 .. 1].
	 */
	operator vec4() const;

	/**
	 * Casts the color to an integer 4-component vector, returned as
	 * [ r, g, b, a ], with each component in the interval [0 .. 255].
	 */
	operator ivec4() const;

	bool operator==(const Color& color) const;
	bool operator!=(const Color& color) const;
	bool operator<(const Color& color) const;

	/**
	 * Returns the red component of this color.
	 */
	float Red() const;

	/**
	 * Returns the green component of this color.
	 */
	float Green() const;

	/**
	 * Returns the blue component of this color.
	 */
	float Blue() const;

	/**
	 * Returns the alpha component of this color.
	 */
	float Alpha() const;

private:
	float _r, _g, _b, _a;

};

}

#endif
