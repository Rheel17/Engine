/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Camera.h"

namespace rheel {

Camera::Camera(std::string name) :
		_name(std::move(name)) {}

void Camera::TransformChanged() {
	_has_view_matrix = false;
}

void Camera::Activate() {
	const auto& [iter, insert] = GetParent()->scene->_cameras.insert({ _name, this });
	if (!insert) {
		Log::Error() << "Camera with name already exists: " << _name << std::endl;
	}
}

void Camera::Deactivate() {
	GetParent()->scene->_cameras.erase(_name);
}

mat4 Camera::GetViewMatrix() const {
	if (!_has_view_matrix) {
		_view_matrix = glm::inverse(CalculateAbsoluteTransformationMatrix());
		_has_view_matrix = true;
	}

	return _view_matrix;
}

mat4 Camera::GetRotationMatrix() const {
	return glm::mat4_cast(CalculateAbsoluteTransform().GetRotation());
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
