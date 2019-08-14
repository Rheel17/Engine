#include "Camera.h"

namespace rheel {

Camera::Camera(std::string name, vec3 position, vec3 rotation) :
		_name(std::move(name)),
		_position(std::move(position)),
		_rotation(std::move(rotation)) {

	_CalculateViewMatrix();
}

void Camera::SetPosition(const vec3& position) {
	SetPosition(position.x, position.y, position.z);
}

void Camera::SetPosition(float x, float y, float z) {
	_position.x = x;
	_position.y = y;
	_position.z = z;
	_CalculateViewMatrix();
}

void Camera::Move(const vec3& position) {
	Move(position.x, position.y, position.z);
}

void Camera::Move(float x, float y, float z) {
	_position.x += x;
	_position.y += y;
	_position.z += z;
	_CalculateViewMatrix();
}

const vec3& Camera::Position() const {
	return _position;
}

void Camera::SetRotation(const vec3& rotation) {
	_rotation = rotation;
	_CalculateViewMatrix();
}

void Camera::Rotate(const vec3& rotation) {
	_rotation *= rotation;
	_CalculateViewMatrix();
}

const vec3& Camera::Rotation() const {
	return _rotation;
}

const mat4& Camera::_ViewMatrix() const {
	return _view_matrix;
}

void Camera::_CalculateViewMatrix() {
	mat4 rotation = glm::identity<mat4>();
	rotation = glm::rotate(rotation, _rotation.x, { 1, 0, 0 });
	rotation = glm::rotate(rotation, _rotation.y, { 0, 1, 0 });
	rotation = glm::rotate(rotation, _rotation.z, { 0, 0, 1 });

	_view_matrix = glm::translate(rotation,
			vec3(-_position.x, -_position.y, _position.z));
}

}
