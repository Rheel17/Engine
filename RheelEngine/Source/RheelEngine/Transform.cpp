/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Transform.h"

namespace rheel {

Transform::Transform(vec3 translation, quat rotation, vec3 scale) :
		RigidTransform(std::move(translation), std::move(rotation)),
		_scale(scale) {}

void Transform::SetIdentity() {
	_scale = vec3(1.0f, 1.0f, 1.0f);
	RigidTransform::SetIdentity();
}

const vec3& Transform::GetScale() const {
	return _scale;
}

void Transform::SetScale(vec3 scale) {
	_scale = scale;
	MarkDirty();
}

void Transform::Scale(const vec3& scale) {
	_scale *= scale;
	MarkDirty();
}

mat4 Transform::CalculateMatrix() const {
	mat4 m = RigidTransform::CalculateMatrix();
	return m * glm::scale(glm::identity<mat4>(), _scale);
}

}
