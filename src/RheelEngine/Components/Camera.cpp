/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Camera.h"

namespace rheel {

mat4 Camera::GetViewMatrix() const {
	return glm::inverse(GetEntity().AbsoluteTransform().AsMatrix());
}

mat4 Camera::GetRotationMatrix() const {
	return glm::mat4_cast(GetEntity().AbsoluteTransform().GetRotation());
}

mat4 Camera::CreateMatrix(unsigned width, unsigned height) const {
	return GetProjectionMatrix(width, height) * GetViewMatrix();
}

vec3 Camera::RayDirection(unsigned width, unsigned height, const vec2& pixel) const {
	vec2 ndc = pixel / vec2(width, height);
	ndc *= 2;
	ndc -= 1;
	ndc.y *= -1;

	return RayDirection(ndc, float(width) / float(height));
}

}
