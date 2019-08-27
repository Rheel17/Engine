#include "DirectionalLight.h"

namespace rheel {

DirectionalLight::DirectionalLight(const Color_t& color, const vec3& direction) :
		Light(color),
		_direction(glm::normalize(direction)) {}

DirectionalLight::~DirectionalLight() {}

void DirectionalLight::SetDirection(const vec3& direction) {
	_direction = direction;
}

const vec3& DirectionalLight::Direction() const {
	return _direction;
}

}
