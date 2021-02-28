/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_CAMERASHAKER_H
#define ENGINE_CAMERASHAKER_H

#include <RheelEngine.h>

class CameraShaker : public rheel::Component {

public:
	static constexpr rheel::ComponentId id = 4;

	// for dx and dz:
	// [ amplitude, sine period, exponent factor ]
	CameraShaker(vec3 dx, vec3 dz);

	void Update() override;

private:
	vec3 _reference;
	vec3 _dx;
	vec3 _dz;
	float _t = 0.0f;

};

#endif
