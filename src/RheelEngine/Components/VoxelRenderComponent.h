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
	explicit VoxelRenderComponent(VoxelImage model);

	void Activate() override;
	void Render() override;
	void Deactivate() override;

private:
	 VoxelImage _model;

};

}

#endif
