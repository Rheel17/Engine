/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef DIRECTIONALLIGHT_H_
#define DIRECTIONALLIGHT_H_
#include "../_common.h"

#include "Light.h"

namespace rheel {

class RE_API DirectionalLight : public Light {

public:
	DirectionalLight(Color color, vec3 direction);

	~DirectionalLight();

	void SetDirection(const vec3& direction);
	const vec3& Direction() const;

private:
	vec3 _direction;

};

}

#endif
