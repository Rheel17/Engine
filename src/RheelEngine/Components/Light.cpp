/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Light.h"

namespace rheel {

Light::Light(const Color& color) :
		_color(color),
		_shadow_distance(0.0f) {}

Light::Light(const Color& color, float shadowDistance) :
		_color(color),
		_shadow_distance(shadowDistance) {}

Light::~Light() {}

void Light::Activate() {
	GetParent()->scene->_lights.push_back(this);
}

void Light::SetColor(const Color& color) {
	_color = color;
}

const Color& Light::GetColor() const {
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
