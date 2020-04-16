/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_VOXELLOADER_H
#define RHEELENGINE_VOXELLOADER_H
#include "../../_common.h"

#include "Loader.h"
#include "../VoxelImage.h"

namespace rheel {

class RE_API VoxelLoader : public Loader<VoxelImage> {
	friend class AssetLoader;

protected:
	VoxelImage DoLoad(const std::string& path) const override;

private:
	VoxelLoader() = default;

	static VoxelImage LoadVox_(std::istream& input);

};

}

#endif