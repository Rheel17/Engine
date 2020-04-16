/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_MODEL_H
#define RHEELENGINE_MODEL_H
#include "../_common.h"

#include "Asset.h"

namespace rheel {

struct RE_API model_vertex {
	vec3 position;
	vec3 normal;
	vec2 texture;
};

struct RE_API model_data {
	std::vector<model_vertex> vertices;
	std::vector<unsigned> indices;
};

class RE_API Model : public Asset<model_data> {

public:
	Model(std::vector<model_vertex> vertices, std::vector<unsigned> indices);

	const std::vector<model_vertex>& GetVertices() const;
	const std::vector<unsigned>& GetIndices() const;

};

}

#endif
