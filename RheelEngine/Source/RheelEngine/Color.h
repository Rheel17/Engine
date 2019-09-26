/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef COLOR_H_
#define COLOR_H_
#include "_common.h"

namespace rheel {

struct RE_API Color {
	float r, g, b, a;

	operator vec4() const;

	bool operator==(const Color& color) const;
	bool operator<(const Color& color) const;
};

}

#endif
