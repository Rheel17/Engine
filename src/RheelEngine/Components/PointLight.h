/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_POINTLIGHT_H
#define RHEELENGINE_POINTLIGHT_H
#include "../_common.h"

#include "Light.h"

namespace rheel {

class RE_API PointLight : public Light {

public:
	PointLight(Color color, vec3 position, float attenuation = 0.0f);

	~PointLight();

	void SetPosition(const vec3& position);
	const vec3& Position() const;

	void SetAttenuation(float attenuation);
	float Attenuation() const;

private:
	vec3 _position;
	float _attenuation;

};

}

#endif
