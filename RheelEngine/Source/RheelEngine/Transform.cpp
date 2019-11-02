/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Transform.h"

namespace rheel {

Transform::Transform(vec3 translation, quat rotation, vec3 scale) :
		RigidTransform(std::move(translation), std::move(rotation)),
		_scale(scale) {}

Transform::Transform(const mat4& matrix) {
	SetTranslation({ matrix[3][0], matrix[3][1], matrix[3][2] });

	vec3 xScaleVector(matrix[0][0], matrix[0][1], matrix[0][2]);
	vec3 yScaleVector(matrix[1][0], matrix[1][1], matrix[1][2]);
	vec3 zScaleVector(matrix[2][0], matrix[2][1], matrix[2][2]);

	_scale.x = glm::dot(xScaleVector, xScaleVector);
	_scale.y = glm::dot(yScaleVector, yScaleVector);
	_scale.z = glm::dot(zScaleVector, zScaleVector);

	mat3 rotationMatrix;
	for (int i = 0; i < 3; i++) {
		rotationMatrix[0][i] = matrix[0][i] / _scale.x;
		rotationMatrix[1][i] = matrix[1][i] / _scale.y;
		rotationMatrix[2][i] = matrix[2][i] / _scale.z;
	}

	SetRotation(rotationMatrix);
}

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
