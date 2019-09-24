#include "PerspectiveCamera.h"

namespace rheel {

PerspectiveCamera::PerspectiveCamera(std::string name, vec3 position, vec3 rotation, float fov, float near, float far) :
		Camera(std::move(name), std::move(position), std::move(rotation)),
		_fov(glm::radians(fov)), _near(near), _far(far) {}

mat4 PerspectiveCamera::CreateMatrix(unsigned width, unsigned height) const {
	return glm::perspective(_fov, float(width) / float(height), _near, _far) * ViewMatrix();
}

std::array<vec3, 8> PerspectiveCamera::ViewspaceCorners(unsigned width, unsigned height, float near, float far) const {
	quat rotation = quat(Rotation());

	vec3 forward = rotation * vec4(0, 0, -1, 0);
	vec3 up = rotation * vec4(0, 1, 0, 0);
	vec3 right = glm::cross(forward, up);

	near = std::max(near, _near);
	far = std::min(far, _far);

	const auto position = [forward, up, right, this](const vec2& ndc, float t) {
		vec3 direction = glm::normalize(ndc.x * right + ndc.y * up + forward);
		return Position() + t * direction;
	};

	return std::array<vec3, 8> {{
		position({ -1, -1 }, near),
		position({ -1,  1 }, near),
		position({  1, -1 }, near),
		position({  1,  1 }, near),
		position({ -1, -1 }, far),
		position({ -1,  1 }, far),
		position({  1, -1 }, far),
		position({  1,  1 }, far),
	}};
}

vec3 PerspectiveCamera::RayDirection(const vec2& ndc, float aspectRatio) const {
	std::cout << ndc << std::endl;

	quat rotation = quat(Rotation());

	vec3 forward = rotation * vec4(0, 0, -1, 0);
	vec3 up = rotation * vec4(0, 1, 0, 0);
	vec3 right = glm::cross(forward, up);

	return glm::normalize(ndc.x * right + ndc.y * up + forward);
}

}
