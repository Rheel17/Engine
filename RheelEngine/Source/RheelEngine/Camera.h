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
	Camera(std::string name, vec3 position, quat rotation);

	virtual ~Camera() = default;

	/**
	 * Creates the matrix which transforms vertices from world space into view
	 * space.
	 */
	virtual mat4 CreateMatrix(unsigned width, unsigned height) const = 0;

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
	void SetRotation(const quat& rotation);

	/**
	 * Rotates this camera relative to its current rotation.
	 */
	void Rotate(const quat& rotation);

	/**
	 * Returns the rotation of this camera.
	 */
	const quat& Rotation() const;

protected:

	/**
	 * Returns the view matrix of this camera, defined by the position and
	 * rotation.
	 */
	const mat4& _ViewMatrix() const;

private:
	void _CalculateViewMatrix();

	std::string _name;
	vec3 _position;
	quat _rotation;

	mat4 _view_matrix;

};

using CameraPtr = std::shared_ptr<Camera>;

}

#endif
