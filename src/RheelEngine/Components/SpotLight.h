/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_SPOTLIGHT_H
#define RHEELENGINE_SPOTLIGHT_H
#include "../_common.h"

#include "Light.h"

namespace rheel {

class RE_API SpotLight : public Light {

public:
	// gen_component_id

	SpotLight(Color color, vec3 position, vec3 direction, float spotAttenuation = 1.0f, float attenuation = 0.0f);
	~SpotLight();

	void SetPosition(const vec3& position);
	const vec3& Position() const;

	void SetDirection(const vec3& direction);
	const vec3& Direction() const;

	void SetSpotAttenuation(float spotAttenuation);
	float SpotAttenuation() const;

	void SetAttenuation(float attenuation);
	float Attenuation() const;

private:
	vec3 _position;
	vec3 _direction;
	float _spot_attenuation;
	float _attenuation;

};

}

#endif
