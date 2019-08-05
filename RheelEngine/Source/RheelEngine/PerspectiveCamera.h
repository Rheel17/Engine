#ifndef PERSPECTIVECAMERA_H_
#define PERSPECTIVECAMERA_H_
#include "_common.h"

#include "Camera.h"

namespace rheel {

class RE_API PerspectiveCamera : public Camera {

public:
	PerspectiveCamera(std::string name, vec3 position, quat rotation, float fov, float near, float far);

	mat4 CreateMatrix(unsigned width, unsigned height) const override;

private:
	float _fov;
	float _near;
	float _far;

};

}

#endif
