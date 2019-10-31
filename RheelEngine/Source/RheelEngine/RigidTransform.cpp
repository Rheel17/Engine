/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "RigidTransform.h"

namespace rheel {

RigidTransform::RigidTransform(vec3 translation, quat rotation) :
		_translation(translation), _rotation(rotation),
		_matrix_dirty(true) {}

RigidTransform::RigidTransform(mat4 matrix) {
	// TODO: transform from matrix
}

void RigidTransform::SetIdentity() {
	_translation = vec3(0.0f, 0.0f, 0.0f);
	_rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);

	_matrix = glm::identity<mat4>();
	_matrix_dirty = false;
}

const vec3& RigidTransform::GetTranslation() const {
	return _translation;
}

void RigidTransform::SetTranslation(vec3 translation) {
	_translation = std::move(translation);
	_matrix_dirty = true;
}

void RigidTransform::Move(const vec3& vec) {
	_translation += vec;
	_matrix_dirty = true;
}

const quat& RigidTransform::GetRotation() const {
	return _rotation;
}

void RigidTransform::SetRotation(quat rotation) {
	_rotation = rotation;
	_matrix_dirty = true;
}

void RigidTransform::Rotate(const quat& rotation) {
	_rotation = rotation * _rotation;
	_matrix_dirty = true;
}

const mat4& RigidTransform::AsMatrix() const {
	if (_matrix_dirty) {
		_matrix = CalculateMatrix();
		_matrix_dirty = false;
	}

	return _matrix;
}

vec3 RigidTransform::ForwardVector() const {
	return _rotation * vec3(0, 0, -1);
}

vec3 RigidTransform::UpVector() const {
	return _rotation * vec3(0, 1, 0);
}

vec3 RigidTransform::RightVector() const {
	return _rotation * vec3(1, 0, 0);
}

void RigidTransform::MarkDirty() {
	_matrix_dirty = true;
}

mat4 RigidTransform::CalculateMatrix() const {
	mat4 m = glm::mat4_cast(_rotation);
	m[3][0] = _translation.x;
	m[3][1] = _translation.y;
	m[3][2] = _translation.z;

	return m;
}

}
