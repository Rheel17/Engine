/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "PerspectiveCamera.h"

namespace rheel {

PerspectiveCamera::PerspectiveCamera(std::string name, float fov, float near, float far) :
		Camera(std::move(name)),
		_fov(glm::radians(fov)), _near(near), _far(far) {}

mat4 PerspectiveCamera::CreateMatrix(unsigned width, unsigned height) const {
	return glm::perspective(_fov, float(width) / float(height), _near, _far) * transform.AsMatrix();
}

std::array<vec3, 8> PerspectiveCamera::ViewspaceCorners(unsigned width, unsigned height, float near, float far) const {
	const quat& rotation = transform.GetRotation();

	vec3 forward = rotation * vec4(0, 0, -1, 0);
	vec3 up = rotation * vec4(0, 1, 0, 0);
	vec3 right = glm::cross(forward, up);

	near = std::max(near, _near);
	far = std::min(far, _far);

	float tanfov = std::tan(_fov * 0.5f);
	float aspectRatio = float(width) / float(height);

	const auto position = [forward, up, right, tanfov, aspectRatio, this](const vec2& ndc, float t) {
		vec3 direction = glm::normalize(
				ndc.x * right * tanfov * aspectRatio +
				ndc.y * up * tanfov +
				forward);

		return transform.GetTranslation() + t * direction;
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
	const quat& rotation = transform.GetRotation();

	vec3 forward = rotation * vec4(0, 0, -1, 0);
	vec3 up = rotation * vec4(0, 1, 0, 0);
	vec3 right = glm::cross(forward, up);
	float tanfov = std::tan(_fov * 0.5f);

	return glm::normalize(
			ndc.x * right * tanfov * aspectRatio +
			ndc.y * up * tanfov +
			forward);
}

}
