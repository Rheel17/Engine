#ifndef CAMERA_H_
#define CAMERA_H_
#include "_common.h"

#include <memory>

namespace rheel {

class RE_API Camera {

public:
	/**
	 * Creates a camera object with a starting position and rotation.
	 */
	Camera(std::string name, vec3 position, vec3 rotation);

	virtual ~Camera() = default;

	/**
	 * Creates the matrix which transforms vertices from world space into view
	 * space.
	 */
	virtual mat4 CreateMatrix(unsigned width, unsigned height) const = 0;

	/**
	 * Returns an array of 8 vertices at the corners of the view space of this
	 * camera.
	 */
	virtual std::array<vec3, 8> ViewspaceCorners(unsigned width, unsigned height) const = 0;

	/**
	 * Sets the position of this camera.
	 */
	void SetPosition(const vec3& position);

	/**
	 * Sets the position of this camera.
	 */
	void SetPosition(float x, float y, float z);

	/**
	 * Moves this object relative to its current position.
	 */
	void Move(const vec3& position);

	/**
	 * Moves this camera relative to its current position.
	 */
	void Move(float x, float y, float z);

	/**
	 * Returns the position of this camera.
	 */
	const vec3& Position() const;

	/**
	 * Sets the rotation of this camera.
	 */
	void SetRotation(const vec3& rotation);

	/**
	 * Rotates this camera relative to its current rotation.
	 */
	void Rotate(const vec3& rotation);

	/**
	 * Returns the rotation of this camera.
	 */
	const vec3& Rotation() const;

	/**
	 * Returns the rotation matrix of this camera.
	 */
	const mat4& RotationMatrix() const;

	/**
	 * Returns the view matrix of this camera, defined by the position and
	 * rotation.
	 */
	const mat4& ViewMatrix() const;

private:
	void _CalculateRotationMatrix() const;
	void _CalculateViewMatrix() const;

	std::string _name;
	vec3 _position;
	vec3 _rotation;

	mutable mat4 _rotation_matrix;
	mutable mat4 _view_matrix;
	mutable bool _rotation_dirty;

};

using CameraPtr = std::shared_ptr<Camera>;

}

#endif
