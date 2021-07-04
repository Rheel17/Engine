/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Model.h"

namespace rheel {

Model::Model(std::vector<model_vertex> vertices, std::vector<unsigned> indices, RenderType render_type) :
		Asset({ std::move(vertices), std::move(indices), render_type }) {}

const std::vector<model_vertex>& Model::GetVertices() const {
	return GetRaw()->vertices;
}

const std::vector<unsigned>& Model::GetIndices() const {
	return GetRaw()->indices;
}

RenderType Model::GetRenderType() const {
	return GetRaw()->render_type;
}

}
