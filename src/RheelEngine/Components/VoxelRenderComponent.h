/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_VOXELRENDERCOMPONENT_H
#define RHEELENGINE_VOXELRENDERCOMPONENT_H
#include "../_common.h"

#include "../Component.h"
#include "../Assets/VoxelImage.h"

namespace rheel {

class RE_API VoxelRenderComponent : public Component {

public:
	// gen_component_id

	explicit VoxelRenderComponent(VoxelImage model);

private:
	VoxelImage _model;

};

}

#endif
