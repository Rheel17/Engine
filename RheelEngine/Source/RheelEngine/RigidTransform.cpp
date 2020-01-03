/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "RigidTransform.h"

namespace rheel {

RigidTransform::RigidTransform(vec3 translation, quat rotation) :
		_translation(translation), _rotation(rotation),
		_matrix_dirty(true) {}

RigidTransform::RigidTransform(TransformOwner *owner) :
		_translation(0.0f, 0.0f, 0.0f), _rotation(1.0f, 0.0f, 0.0f, 0.0f),
		_matrix_dirty(true), _owner(owner) {}

RigidTransform::RigidTransform(const RigidTransform& t) :
		_translation(t._translation), _rotation(t._rotation),
		_matrix_dirty(true) {}

RigidTransform::RigidTransform(RigidTransform&& t) :
		_translation(std::move(t._translation)),
		_rotation(std::move(t._rotation)),
		_matrix_dirty(true) {}

RigidTransform& RigidTransform::operator=(const RigidTransform& t) {
	_translation = t._translation;
	_rotation = t._rotation;
	SetChanged();
	return *this;
}

RigidTransform& RigidTransform::operator=(RigidTransform&& t) {
	_translation = std::move(t._translation);
	_rotation = std::move(t._rotation);
	SetChanged();
	return *this;
}

void RigidTransform::SetIdentity() {
	_translation = vec3(0.0f, 0.0f, 0.0f);
	_rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
	SetChanged();
}

const vec3& RigidTransform::GetTranslation() const {
	return _translation;
}

void RigidTransform::SetTranslation(vec3 translation) {
	if (_translation != translation) {
		_translation = std::move(translation);
		SetChanged();
	}
}

void RigidTransform::Move(const vec3& vec) {
	SetTranslation(_translation + vec);
}

const quat& RigidTransform::GetRotation() const {
	return _rotation;
}

void RigidTransform::SetRotation(quat rotation) {
	if (_rotation != rotation) {
		_rotation = rotation;
		SetChanged();
	}
}

void RigidTransform::Rotate(const quat& rotation) {
	SetRotation(rotation * _rotation);
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

void RigidTransform::SetChanged() {
	_matrix_dirty = true;

	if (_owner) {
		_owner->TransformChanged();
	}
}

mat4 RigidTransform::CalculateMatrix() const {
	mat4 m = glm::mat4_cast(_rotation);
	m[3][0] = _translation.x;
	m[3][1] = _translation.y;
	m[3][2] = _translation.z;

	return m;
}

}
