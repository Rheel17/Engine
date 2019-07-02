#ifndef CAMERA_H_
#define CAMERA_H_
#include "_common.h"

#include <memory>

namespace rheel {

class RE_API Camera {

public:
	Camera(std::string name, vec3 position, vec3 rotation);
	virtual ~Camera() = default;

	virtual mat4 GetMatrix(unsigned width, unsigned height) const = 0;

	const vec3& Position() const;

protected:
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
