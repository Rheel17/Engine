/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef CAMERA_H_
#define CAMERA_H_
#include "../_common.h"

#include "../Component.h"

#include <array>

namespace rheel {

class RE_API Camera : public RigidComponent {

public:
	/**
	 * Creates a camera object with a starting position and rotation.
	 */
	Camera(std::string name);

	void TransformChanged() override;

	void Activate() override;

	virtual ~Camera() = default;

	/**
	 * Returns the view matrix of this camera.
	 */
	mat4 GetViewMatrix() const;

	/**
	 * Creates the matrix which transforms vertices from world space into view
	 * space.
	 */
	virtual mat4 CreateMatrix(unsigned width, unsigned height) const = 0;

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
	std::string _name;

	mutable mat4 _view_matrix;
	mutable bool _has_view_matrix = false;

};

}

#endif
