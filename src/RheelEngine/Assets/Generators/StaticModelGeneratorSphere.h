/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_STATICMODELGENERATORSPHERE_H
#define RHEELENGINE_STATICMODELGENERATORSPHERE_H
#include "../../_common.h"

#include "StaticModelGenerator.h"

namespace rheel {

class RE_API StaticModelGeneratorSphere : public StaticModelGenerator {

public:
	explicit StaticModelGeneratorSphere(float radius, unsigned subdivisions = 16);

	void DoGenerate() override;

private:
	float _radius;
	unsigned _subdivisions;

};

}

#endif
