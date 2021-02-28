/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "CameraShaker.h"

CameraShaker::CameraShaker(vec3 dx, vec3 dz) :
		_dx(dx),
		_dz(dz) {}

void CameraShaker::Update() {
	if (_t == 0.0f) {
		_reference = GetEntity().transform.GetTranslation();
	}

	_t += dt;
	float f_x = _dx.x * std::sin(_dx.y * _t) * std::exp(-_dx.z * _t);
	float f_z = _dz.x * std::sin(_dz.y * _t) * std::exp(-_dz.z * _t);

	GetEntity().transform.SetTranslation(_reference + vec3{ f_x, 0.0f, f_z });
}
