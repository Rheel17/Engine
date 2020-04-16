/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PerspectiveCamera.h"

namespace rheel {

PerspectiveCamera::PerspectiveCamera(std::string name, float fov, float near, float far) :
		Camera(std::move(name)),
		_fov(glm::radians(fov)),
		_near(near),
		_far(far) {}

float PerspectiveCamera::GetSkyboxDistance() const {
	return _far / sqrt(3);
}

mat4 PerspectiveCamera::GetProjectionMatrix(unsigned width, unsigned height) const {
	// TODO: cache perspective matrix. Width and Height don't usually change.
	// TODO: notify components of transform change? This could optimize this
	// code a lot. Maybe also give the source of the transform change. However,
	// that would require the restructure of a lot of transform code.
	return glm::perspective(_fov, float(width) / float(height), _near, _far);
}

std::array<vec3, 8> PerspectiveCamera::ViewspaceCorners(unsigned width, unsigned height, float near, float far) const {
	Transform absoluteTransform = CalculateAbsoluteTransform();
	const quat& rotation = absoluteTransform.GetRotation();

	vec3 forward = rotation * vec4(0, 0, -1, 0);
	vec3 up = rotation * vec4(0, 1, 0, 0);
	vec3 right = glm::cross(forward, up);

	near = std::max(near, _near);
	far = std::min(far, _far);

	float tanfov = std::tan(_fov * 0.5f);
	float aspectRatio = float(width) / float(height);

	const auto position = [absoluteTransform, forward, up, right, tanfov, aspectRatio](const vec2& ndc, float t) {
		vec3 direction = glm::normalize(
				ndc.x * right * tanfov * aspectRatio +
						ndc.y * up * tanfov +
						forward);

		return absoluteTransform.GetTranslation() + t * direction;
	};

	return std::array<vec3, 8>{ {
			position({ -1.0f, -1.0f }, near),
			position({ -1.0f,  1.0f }, near),
			position({  1.0f, -1.0f }, near),
			position({  1.0f,  1.0f }, near),
			position({ -1.0f, -1.0f }, far),
			position({ -1.0f,  1.0f }, far),
			position({  1.0f, -1.0f }, far),
			position({  1.0f,  1.0f }, far),
	} };
}

vec3 PerspectiveCamera::RayDirection(const vec2& ndc, float aspectRatio) const {
	const quat& rotation = transform.GetRotation();

	vec3 forward = rotation * vec4(0, 0, -1, 0);
	vec3 up = rotation * vec4(0, 1, 0, 0);
	vec3 right = glm::cross(forward, up);
	float tanfov = std::tan(_fov * 0.5f);

	return glm::normalize(ndc.x * right * tanfov * aspectRatio + ndc.y * up * tanfov + forward);
}

}
