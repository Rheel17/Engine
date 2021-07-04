/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_MODEL_H
#define RHEELENGINE_MODEL_H
#include "../_common.h"

#include "Asset.h"

namespace rheel {

enum class RenderType {
	TRIANGLES, LINES
};

struct model_vertex {
	vec3 position;
	vec3 normal;
	vec2 texture;
};

struct model_data {
	std::vector<model_vertex> vertices;
	std::vector<unsigned> indices;
	RenderType render_type;
};

class RE_API Model : public Asset<model_data> {

public:
	Model(std::vector<model_vertex> vertices, std::vector<unsigned> indices, RenderType render_type = RenderType::TRIANGLES);

	const std::vector<model_vertex>& GetVertices() const;
	const std::vector<unsigned>& GetIndices() const;
	RenderType GetRenderType() const;

};

}

#endif
