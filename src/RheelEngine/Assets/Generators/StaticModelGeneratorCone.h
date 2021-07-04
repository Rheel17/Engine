/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_STATICMODELGENERATORCONE_H
#define ENGINE_STATICMODELGENERATORCONE_H
#include "../../_common.h"

#include "StaticModelGenerator.h"

namespace rheel {

class StaticModelGeneratorCone : public StaticModelGenerator {

public:
	StaticModelGeneratorCone(float radius, float height, unsigned int subdivisions = 16);

	void DoGenerate() override;

private:
	float _radius;
	float _height;
	unsigned int _subdivisions;

};

}

#endif
