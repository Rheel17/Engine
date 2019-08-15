#include "PerspectiveCamera.h"

namespace rheel {

PerspectiveCamera::PerspectiveCamera(std::string name, vec3 position, vec3 rotation, float fov, float near, float far) :
		Camera(std::move(name), std::move(position), std::move(rotation)),
		_fov(glm::radians(fov)), _near(near), _far(far) {}

mat4 PerspectiveCamera::CreateMatrix(unsigned width, unsigned height) const {
	return glm::perspective(_fov, float(width) / float(height), _near, _far) * ViewMatrix();
}

}
