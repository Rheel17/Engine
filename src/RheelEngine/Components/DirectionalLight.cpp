/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "DirectionalLight.h"

namespace rheel {

DirectionalLight::DirectionalLight(const Color& color, const vec3& direction) :
		Light(color),
		_direction(glm::normalize(direction)) {}

void DirectionalLight::SetDirection(const vec3& direction) {
	_direction = direction;
}

const vec3& DirectionalLight::Direction() const {
	return _direction;
}

}
