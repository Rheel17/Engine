#include "Light.h"

namespace rheel {

Light::Light(vec3 position, Color_t color, float attenuation, bool castsShadows) :
		_type(PointLight),
		_position(std::move(position)), _color(std::move(color)),
		_attenuation(attenuation),
		_casts_shadows(castsShadows) {}

Light::Light(vec3 position, Color_t color, vec3 direction, float spotAttenuation, float attenuation, bool castsShadows) :
		_type(SpotLight),
		_position(std::move(position)), _direction(std::move(direction)),
		_color(std::move(color)),
		_attenuation(attenuation), _spot_attenuation(spotAttenuation),
		_casts_shadows(castsShadows) {}

Light::Light(Color_t color, vec3 direction, bool castsShadows) :
		_type(DirectionalLight), _direction(std::move(direction)),
		_color(std::move(color)),
		_casts_shadows(castsShadows) {}

Light::LightType Light::Type() const {
	return _type;
}

const vec3& Light::Position() const {
	return _position;
}

const vec3& Light::Direction() const {
	return _direction;
}

const Light::Color_t& Light::Color() const {
	return _color;
}

float Light::Attenuation() const {
	return _attenuation;
}

float Light::SpotAttenuation() const {
	return _spot_attenuation;
}

bool Light::CastsShadows() const {
	return _casts_shadows;
}

}
