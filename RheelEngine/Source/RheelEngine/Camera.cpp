#include "Camera.h"

namespace rheel {

Camera::Camera(std::string name, vec3 position, quat rotation) :
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
}

void Camera::Move(const vec3& position) {
	Move(position.x, position.y, position.z);
}

void Camera::Move(float x, float y, float z) {
	_position.x += x;
	_position.y += y;
	_position.z += z;
}

const vec3& Camera::Position() const {
	return _position;
}

void Camera::SetRotation(const quat& rotation) {
	_rotation = rotation;
}

void Camera::Rotate(const quat& rotation) {
	_rotation *= rotation;
}

const quat& Camera::Rotation() const {
	return _rotation;
}

const mat4& Camera::_ViewMatrix() const {
	return _view_matrix;
}

void Camera::_CalculateViewMatrix() {
	_view_matrix = glm::translate(
			glm::mat4_cast(_rotation),
			vec3(-_position.x, -_position.y, _position.z));
}

}
