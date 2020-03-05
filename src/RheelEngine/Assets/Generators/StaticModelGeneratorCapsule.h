/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_STATICMODELGENERATORCAPSULE_H
#define RHEELENGINE_STATICMODELGENERATORCAPSULE_H
#include "../../_common.h"

#include "StaticModelGenerator.h"

namespace rheel {

class RE_API StaticModelGeneratorCapsule : public StaticModelGenerator {

public:
	StaticModelGeneratorCapsule(float radius, float straightHeight, unsigned subdivisions = 16);

	void DoGenerate() override;

private:
	float _radius;
	float _straight_height;
	unsigned _subdivisions;

};

}

#endif
