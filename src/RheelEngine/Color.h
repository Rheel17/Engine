/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_COLOR_H
#define RHEELENGINE_COLOR_H
#include "_common.h"

namespace rheel {

class RE_API Color {

public:
	float r, g, b, a;

	operator vec4() const;

	bool operator==(const Color& color) const;
	bool operator<(const Color& color) const;
};

}

#endif
