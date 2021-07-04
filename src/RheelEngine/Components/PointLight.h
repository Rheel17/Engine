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
	// gen_component_id
	static constexpr const ComponentId id = 65289;
	static constexpr const ComponentFlags flags = ComponentFlags::BUILTIN;

	PointLight(const Color& color, const vec3& position, float attenuation = 0.0f);

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
