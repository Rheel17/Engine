#include "Camera.h"

namespace rheel {

Camera::Camera(std::string name, vec3 position, vec3 rotation) :
		_name(std::move(name)),
		_position(std::move(position)),
		_rotation(std::move(rotation)) {

	_position.x *= -1;
	_position.y *= -1;

	_view_matrix = glm::identity<mat4>();
	_view_matrix = glm::rotate(_view_matrix, glm::radians(_rotation.x), { 1.0f, 0.0f, 0.0f });
	_view_matrix = glm::rotate(_view_matrix, glm::radians(_rotation.y), { 0.0f, 1.0f, 0.0f });
	_view_matrix = glm::rotate(_view_matrix, glm::radians(_rotation.z), { 0.0f, 0.0f, 1.0f });
	_view_matrix = _view_matrix * glm::translate(glm::identity<mat4>(), _position);
}

const mat4& Camera::_ViewMatrix() const {
	return _view_matrix;
}

const vec3& Camera::Position() const {
	return _position;
}

}
