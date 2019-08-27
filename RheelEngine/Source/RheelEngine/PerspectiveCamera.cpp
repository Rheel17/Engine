#include "PerspectiveCamera.h"

namespace rheel {

PerspectiveCamera::PerspectiveCamera(std::string name, vec3 position, vec3 rotation, float fov, float near, float far) :
		Camera(std::move(name), std::move(position), std::move(rotation)),
		_fov(glm::radians(fov)), _near(near), _far(far) {}

mat4 PerspectiveCamera::CreateMatrix(unsigned width, unsigned height) const {
	return glm::perspective(_fov, float(width) / float(height), _near, _far) * ViewMatrix();
}

std::array<vec3, 8> PerspectiveCamera::ViewspaceCorners(unsigned width, unsigned height) const {
	quat rotation = quat(Rotation());

	vec3 forward = rotation * vec4(0, 0, -1, 0);
	vec3 up = rotation * vec4(0, 1, 0, 0);
	vec3 right = glm::cross(forward, up);

	float tanfov = std::tan(_fov);
	float aspect = float(width) / float(height);

	float farWidth = _far * tanfov;
	float nearWidth = _near * tanfov;
	float farHeight = farWidth / aspect;
	float nearHeight = nearWidth / aspect;

	vec3 centerNear = Position() + _near * forward;
	vec3 centerFar = Position() + _far * forward;

	vec3 farTop = centerFar + farHeight * up;
	vec3 farBottom = centerFar - farHeight * up;
	vec3 nearTop = centerNear + nearHeight * up;
	vec3 nearBottom = centerNear - nearHeight * up;

	return std::array<vec3, 8> {{
		farTop + right * farWidth,
		farTop - right * farWidth,
		farBottom + right * farWidth,
		farBottom - right * farWidth,
		nearTop + right * nearWidth,
		nearTop - right * nearWidth,
		nearBottom + right * nearWidth,
		nearBottom - right * nearWidth,
	}};
}

}
