/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_MODELGENERATOR_H
#define RHEELENGINE_MODELGENERATOR_H
#include "../../_common.h"

#include "../Model.h"

namespace rheel {

class RE_API ModelGenerator {

public:
	RE_NO_CONSTRUCT(ModelGenerator)

public:
	static Model Box(vec3 dimensions);
	static Model Sphere(float radius, unsigned subdivisions = 16);
	static Model Capsule(float radius, float straightHeight, unsigned subdivisions = 16);


};

}

#endif