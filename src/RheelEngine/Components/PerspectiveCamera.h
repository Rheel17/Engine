/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_PERSPECTIVECAMERA_H
#define RHEELENGINE_PERSPECTIVECAMERA_H
#include "../_common.h"

#include "Camera.h"

namespace rheel {

class RE_API PerspectiveCamera : public Camera {

public:
	// gen_component_id

	PerspectiveCamera(float fov, float near, float far);

	float GetSkyboxDistance() const override;

	mat4 GetProjectionMatrix(unsigned width, unsigned height) const override;

	std::array<vec3, 8> ViewspaceCorners(unsigned width, unsigned height, float near, float far) const override;

	vec3 RayDirection(const vec2& ndc, float aspect_ratio) const override;

private:
	float _fov;
	float _near;
	float _far;

};

}

#endif
