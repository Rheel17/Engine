/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "VoxelRenderComponent.h"

namespace rheel {

VoxelRenderComponent::VoxelRenderComponent(VoxelImage model) :
		_model(std::move(model)) {}

}