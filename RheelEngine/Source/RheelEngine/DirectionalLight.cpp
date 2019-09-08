#include "DirectionalLight.h"

namespace rheel {

DirectionalLight::DirectionalLight(Color color, vec3 direction) :
		Light(std::move(color)),
		_direction(glm::normalize(std::move(direction))) {}

DirectionalLight::~DirectionalLight() {}

void DirectionalLight::SetDirection(const vec3& direction) {
	_direction = direction;
}

const vec3& DirectionalLight::Direction() const {
	return _direction;
}

}
