#include "Camera.h"

namespace rheel {

Camera::Camera(std::string name, vec3 position, vec3 rotation) :
		_name(std::move(name)),
		_position(std::move(position)),
		_rotation(std::move(rotation)),
		_rotation_dirty(true),
		_position_dirty(true) {

	_CalculateViewMatrix();
}

void Camera::SetPosition(const vec3& position) {
	SetPosition(position.x, position.y, position.z);
}

void Camera::SetPosition(float x, float y, float z) {
	_position.x = x;
	_position.y = y;
	_position.z = z;
	_position_dirty = true;
}

void Camera::Move(const vec3& position) {
	Move(position.x, position.y, position.z);
}

void Camera::Move(float x, float y, float z) {
	_position.x += x;
	_position.y += y;
	_position.z += z;
	_position_dirty = true;
}

const vec3& Camera::Position() const {
	return _position;
}

void Camera::SetRotation(const vec3& rotation) {
	_rotation = rotation;
	_rotation_dirty = true;
}

void Camera::Rotate(const vec3& rotation) {
	_rotation *= rotation;
	_rotation_dirty = true;
}

const vec3& Camera::Rotation() const {
	return _rotation;
}

const mat4& Camera::RotationMatrix() const {
	_CalculateRotationMatrix();
	return _rotation_matrix;
}

const mat4& Camera::ViewMatrix() const {
	_CalculateViewMatrix();
	return _view_matrix;
}

void Camera::_CalculateRotationMatrix() const {
	if (!_rotation_dirty) {
		return;
	}

	_rotation_matrix = glm::identity<mat4>();
	_rotation_matrix = glm::rotate(_rotation_matrix, _rotation.x, { 1, 0, 0 });
	_rotation_matrix = glm::rotate(_rotation_matrix, _rotation.y, { 0, 1, 0 });
	_rotation_matrix = glm::rotate(_rotation_matrix, _rotation.z, { 0, 0, 1 });

	_rotation_dirty = false;
}

void Camera::_CalculateViewMatrix() const {
	if (!_rotation_dirty && !_position_dirty) {
		return;
	}

	_CalculateRotationMatrix();

	_view_matrix = glm::translate(_rotation_matrix,
			vec3(-_position.x, -_position.y, _position.z));

	_position_dirty = false;
}

}
