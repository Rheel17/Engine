/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include "_common.h"

namespace rheel {

class RE_API Transform {

public:
	Transform(
			vec3 translation = { 0.0f, 0.0f, 0.0f },
			quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f },
			vec3 scale = { 1.0f, 1.0f, 1.0f });

	vec3 translation;
	quat rotation;
	vec3 scale;
};

}

#endif
