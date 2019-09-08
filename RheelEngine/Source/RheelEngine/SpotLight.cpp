#include "SpotLight.h"

namespace rheel {

SpotLight::SpotLight(Color color, vec3 position, vec3 direction, float spotAttenuation, float attenuation) :
		Light(std::move(color)),
		_position(std::move(position)), _direction(glm::normalize(std::move(direction))),
		_spot_attenuation(spotAttenuation), _attenuation(attenuation) {}

SpotLight::~SpotLight() {}

void SpotLight::SetPosition(const vec3& position) {
	_position = position;
}

const vec3& SpotLight::Position() const {
	return _position;
}

void SpotLight::SetDirection(const vec3& direction) {
	_direction = direction;
}

const vec3& SpotLight::Direction() const {
	return _direction;
}

void SpotLight::SetSpotAttenuation(float spotAttenuation) {
	_spot_attenuation = spotAttenuation;
}

float SpotLight::SpotAttenuation() const {
	return _spot_attenuation;
}

void SpotLight::SetAttenuation(float attenuation) {
	_attenuation = attenuation;
}

float SpotLight::Attenuation() const {
	return _attenuation;
}

}
