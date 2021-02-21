/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_VOXELLOADER_H
#define RHEELENGINE_VOXELLOADER_H
#include "../../_common.h"

#include "Loader.h"
#include "../VoxelImage.h"

namespace rheel {

class RE_API VoxelLoader : public AbstractLoader<VoxelImage> {
	friend class AssetLoader;

public:
	VoxelImage Load(const std::string& path) const override;

private:
	static VoxelImage _load_vox(std::istream& input);

};

}

#endif