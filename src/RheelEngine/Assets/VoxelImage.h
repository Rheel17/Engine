/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_VOXELIMAGE_H
#define RHEELENGINE_VOXELIMAGE_H
#include "../_common.h"

#include "Asset.h"
#include "../Color.h"

namespace rheel {

struct RE_API voxel_image_data {
	unsigned width;
	unsigned height;
	unsigned depth;
	std::vector<Color> voxels;
};

class RE_API VoxelImage : public Asset<voxel_image_data> {

public:
	/**
	 * Creates an empty 3d image with the given dimensions
	 */
	VoxelImage(unsigned width, unsigned height, unsigned depth);
	VoxelImage(unsigned width, unsigned height, unsigned depth, std::vector<Color> voxels);

	unsigned GetWidth() const;
	unsigned GetHeight() const;
	unsigned GetDepth() const;

	const Color& At(unsigned x, unsigned y, unsigned z) const;
	Color& At(unsigned x, unsigned y, unsigned z);

private:
	VoxelImage() = default;

};

}

#endif
