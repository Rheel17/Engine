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
	 * Returns the position of this camera
	 */
	const vec3& Position() const;

	/**
	 * Returns the rotation of this camera
	 */
	const vec3& Rotation() const;

protected:

	/**
	 * Returns the view matrix of this camera, defined by the position and
	 * rotation.
	 */
	const mat4& _ViewMatrix() const;

private:
	std::string _name;
	vec3 _position;
	vec3 _rotation;

	mat4 _view_matrix;

};

using CameraPtr = std::shared_ptr<Camera>;

}

#endif
