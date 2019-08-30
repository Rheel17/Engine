#include "Light.h"

namespace rheel {

Light::Light(const Color_t& color) :
		_color(color), _shadow_distance(0.0f) {}

Light::Light(const Color_t& color, float shadowDistance) :
		_color(color), _shadow_distance(shadowDistance) {}

Light::~Light() {}

void Light::SetColor(const Color_t& color) {
	_color = color;
}

const Light::Color_t& Light::Color() const {
	return _color;
}

void Light::SetShadowDistance(float shadowDistance) {
	_shadow_distance = shadowDistance;
}

float Light::ShadowDistance() const {
	return _shadow_distance;
}

bool Light::CastsShadows() const {
	return _shadow_distance > 0.0f;
}

}
