/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_DIRECTIONALLIGHT_H
#define RHEELENGINE_DIRECTIONALLIGHT_H
#include "../_common.h"

#include "Light.h"

namespace rheel {

class RE_API DirectionalLight : public Light {

public:
	// gen_component_id
	static constexpr const ComponentId id = 65282;
	static constexpr const ComponentFlags flags = ComponentFlags::BUILTIN;

	DirectionalLight(const Color& color, const vec3& direction);

	void SetDirection(const vec3& direction);
	const vec3& Direction() const;

private:
	vec3 _direction;

};

}

#endif
