/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Model.h"

#include <utility>

namespace rheel {

Model::Model(std::vector<model_vertex> vertices, std::vector<unsigned> indices) :
		Asset({ std::move(vertices), std::move(indices) }) {}

const std::vector<model_vertex>& Model::GetVertices() const {
	return GetRaw()->vertices;
}

const std::vector<unsigned>& Model::GetIndices() const {
	return GetRaw()->indices;
}

}
