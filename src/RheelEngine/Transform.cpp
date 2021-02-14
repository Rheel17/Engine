/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Transform.h"

namespace rheel {

Transform::Transform(const vec3& translation, const quat& rotation, const vec3& scale) :
		_translation(translation),
		_rotation(rotation),
		_scale(scale),
		_matrix() {

	CalculateMatrix();
}

Transform::Transform(const mat4& matrix) :
		_translation(matrix[3][0], matrix[3][1], matrix[3][2]),
		_scale([&matrix] {
			vec3 x_scale_vector(matrix[0][0], matrix[0][1], matrix[0][2]);
			vec3 y_scale_vector(matrix[1][0], matrix[1][1], matrix[1][2]);
			vec3 z_scale_vector(matrix[2][0], matrix[2][1], matrix[2][2]);

			return vec3{
					sqrt(glm::dot(x_scale_vector, x_scale_vector)),
					sqrt(glm::dot(y_scale_vector, y_scale_vector)),
					sqrt(glm::dot(z_scale_vector, z_scale_vector))
			};
		}()),
		_rotation([this, &matrix] {
			mat3 rotation_matrix;
			for (int i = 0; i < 3; i++) {
				rotation_matrix[0][i] = matrix[0][i] / _scale.x;
				rotation_matrix[1][i] = matrix[1][i] / _scale.y;
				rotation_matrix[2][i] = matrix[2][i] / _scale.z;
			}

			return quat(rotation_matrix);
		}()),
		_matrix(matrix) {}

void Transform::SetIdentity() {
	_translation = vec3(0.0f, 0.0f, 0.0f);
	_rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
	_scale = vec3(1.0f, 1.0f, 1.0f);
	_matrix = glm::identity<mat4>();
}

const vec3& Transform::GetTranslation() const {
	return _translation;
}

void Transform::SetTranslation(const vec3& translation) {
	if (_translation != translation) {
		_translation = translation;
		_dirty = true;
	}
}

void Transform::Move(const vec3& vec) {
	SetTranslation(_translation + vec);
}

const quat& Transform::GetRotation() const {
	return _rotation;
}

void Transform::SetRotation(const quat& rotation) {
	if (_rotation != rotation) {
		_rotation = rotation;
		_dirty = true;
	}
}

void Transform::Rotate(const quat& rotation) {
	SetRotation(rotation * _rotation);
}

const vec3& Transform::GetScale() const {
	return _scale;
}

void Transform::SetScale(const vec3& scale) {
	if (_scale != scale) {
		_scale = scale;
		_dirty = true;
	}
}

void Transform::Scale(const vec3& scale) {
	SetScale(_scale * scale);
}

const mat4& Transform::AsMatrix() const {
	if (_dirty) {
		_matrix = CalculateMatrix();
		_dirty = false;
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

Transform Transform::operator*(const Transform& t) const {
	return Transform(AsMatrix() * t.AsMatrix());
}

mat4 Transform::CalculateMatrix() const {
	mat4 m = glm::mat4_cast(_rotation);
	m[3][0] = _translation.x;
	m[3][1] = _translation.y;
	m[3][2] = _translation.z;

	return m * glm::scale(glm::identity<mat4>(), _scale);
}

}
