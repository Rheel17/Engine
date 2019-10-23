/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Camera.h"

namespace rheel {

Camera::Camera(std::string name) :
		_name(std::move(name)) {}

void Camera::Activate() {
	// TODO: log warning replace camera

	GetParent()->scene->_cameras.insert({ _name, this });
}

vec3 Camera::RayDirection(unsigned width, unsigned height, const vec2& pixel) const {
	vec2 ndc = pixel / vec2(width, height);
	ndc *= 2;
	ndc -= 1;
	ndc.y *= -1;

	return RayDirection(ndc, float(width) / float(height));
}

}
