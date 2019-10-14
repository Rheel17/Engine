/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Transform.h"

namespace rheel {

Transform::Transform(vec3 translation, quat rotation, vec3 scale) :
		_translation(translation), _rotation(rotation), _scale(scale) {

	_matrix_dirty = true;
}

void Transform::SetIdentity() {
	_translation = vec3(0.0f, 0.0f, 0.0f);
	_rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
	_scale = vec3(1.0f, 1.0f, 1.0f);

	_matrix = glm::identity<mat4>();
	_matrix_dirty = false;
}

const vec3& Transform::GetTranslation() const {
	return _translation;
}

void Transform::SetTranslation(vec3 translation) {
	_translation = std::move(translation);
	_matrix_dirty = true;
}

void Transform::Move(const vec3& vec) {
	_translation += vec;
	_matrix_dirty = true;
}

const quat& Transform::GetRotation() const {
	return _rotation;
}

void Transform::SetRotation(quat rotation) {
	_rotation = rotation;
	_matrix_dirty = true;
}

void Transform::Rotate(const quat& rotation) {
	_rotation = rotation * _rotation;
	_matrix_dirty = true;
}

const vec3& Transform::GetScale() const {
	return _scale;
}

void Transform::SetScale(vec3 scale) {
	_scale = scale;
	_matrix_dirty = true;
}

void Transform::Scale(const vec3& scale) {
	_scale *= scale;
	_matrix_dirty = true;
}

const mat4& Transform::AsMatrix() const {
	if (_matrix_dirty) {
		_matrix = glm::mat4_cast(_rotation);
		_matrix[0][0] *= _scale.x;
		_matrix[1][1] *= _scale.y;
		_matrix[2][2] *= _scale.z;
		_matrix[3][0] = _translation.x;
		_matrix[3][1] = _translation.y;
		_matrix[3][2] = _translation.z;
	}

	return _matrix;
}

vec3 Transform::ForwardVector() const {
	return _rotation * vec3(0, 0, -1);
}

vec3 Transform::UpVector() const {
	return _rotation * vec3(0, 1, 0);
}

vec3 Transform::RightVector() const {
	return _rotation * vec3(1, 0, 0);
}

}

