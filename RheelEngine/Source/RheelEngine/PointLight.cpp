#include "PointLight.h"

namespace rheel {

PointLight::PointLight(const Color_t& color, const vec3& position, float attenuation) :
		Light(color),
		_position(position), _attenuation(attenuation) {}

PointLight::~PointLight() {}

void PointLight::SetPosition(const vec3& position) {
	_position = position;
}

const vec3& PointLight::Position() const {
	return _position;
}

void PointLight::SetAttenuation(float attenuation) {
	_attenuation = attenuation;
}

float PointLight::Attenuation() const {
	return _attenuation;
}

}
