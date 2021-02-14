/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_CAMERA_H
#define RHEELENGINE_CAMERA_H
#include "../_common.h"

#include "../Component.h"
#include "../Scene.h"

#include <array>

namespace rheel {

class RE_API Camera : public Component {

public:
	// gen_component_id

	/**
	 * Returns the skybox distance
	 */
	virtual float GetSkyboxDistance() const = 0;

	/**
	 * Returns the projection matrix of this camera.
	 */
	virtual mat4 GetProjectionMatrix(unsigned width, unsigned height) const = 0;

	/**
	 * Returns the view matrix of this camera.
	 */
	mat4 GetViewMatrix() const;

	/**
	 * Returns the rotation matrix of this camera.
	 */
	mat4 GetRotationMatrix() const;

	/**
	 * Creates the matrix which transforms vertices from world space into view
	 * space.
	 */
	mat4 CreateMatrix(unsigned width, unsigned height) const;

	/**
	 * Returns an array of 8 vertices at the corners of the view space of this
	 * camera.
	 */
	virtual std::array<vec3, 8> ViewspaceCorners(unsigned width, unsigned height, float near, float far) const = 0;

	/**
	 * Returns the direction of a ray starting at the camera origin and going
	 * through the specified normalized device coordinates.
	 */
	virtual vec3 RayDirection(const vec2& ndc, float apsectRatio) const = 0;

	/**
	 * Returns the direction of a ray starting at the camera origin and going
	 * through the given pixel.
	 */
	vec3 RayDirection(unsigned width, unsigned height, const vec2& pixel) const;

private:
	mutable mat4 _view_matrix{};
	mutable bool _has_view_matrix = false;

};

}

#endif
