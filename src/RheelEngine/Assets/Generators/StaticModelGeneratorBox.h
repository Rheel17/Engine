/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_STATICMODELGENERATORBOX_H
#define RHEELENGINE_STATICMODELGENERATORBOX_H
#include "../../_common.h"

#include "StaticModelGenerator.h"

namespace rheel {

class RE_API StaticModelGeneratorBox : public StaticModelGenerator {

public:
	explicit StaticModelGeneratorBox(vec3 dimensions);

	void DoGenerate() override;

private:
	vec3 _dimensions;

};

}

#endif
