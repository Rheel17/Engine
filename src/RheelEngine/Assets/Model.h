/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef MODEL_H_
#define MODEL_H_
#include "../_common.h"

#include "Asset.h"

namespace rheel {

struct RE_API ModelVertex {
	vec3 position;
	vec3 normal;
	vec2 texture;
};

struct RE_API _ModelData {
	std::vector<ModelVertex> vertices;
	std::vector<unsigned> indices;
};

class RE_API Model : public Asset<_ModelData> {

public:
	Model(std::vector<ModelVertex> vertices, std::vector<unsigned> indices);

	const std::vector<ModelVertex>& GetVertices() const;
	const std::vector<unsigned>& GetIndices() const;

};

}

#endif
