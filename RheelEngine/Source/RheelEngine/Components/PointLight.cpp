/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PointLight.h"

namespace rheel {

PointLight::PointLight(Color color, vec3 position, float attenuation) :
		Light(std::move(color)),
		_position(std::move(position)), _attenuation(attenuation) {}

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
