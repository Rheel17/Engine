/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Model.h"

#include <utility>

namespace rheel {

Model::Model(std::vector<ModelVertex> vertices, std::vector<unsigned> indices) :
		Asset({ std::move(vertices), std::move(indices) }) {}

const std::vector<ModelVertex>& Model::GetVertices() const {
	return _GetRaw()->vertices;
}

const std::vector<unsigned>& Model::GetIndices() const {
	return _GetRaw()->indices;
}


}
