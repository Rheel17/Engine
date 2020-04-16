/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_STATICMODELGENERATOR_H
#define RHEELENGINE_STATICMODELGENERATOR_H
#include "../../_common.h"

#include "../Model.h"

namespace rheel {

class RE_API StaticModelGenerator {

public:
	virtual ~StaticModelGenerator() = default;

	/*
	 * Function application invokes the generate() method
	 */
	Model operator()();

	/*
	 * Generates the model
	 */
	Model Generate();

protected:
	/*
	 * Derived classes should populate the vertices and indices vectors using in
	 * the implementation of this method.
	 */
	virtual void DoGenerate() = 0;

	std::vector<model_vertex> vertices;
	std::vector<unsigned> indices;

};

}

#endif
