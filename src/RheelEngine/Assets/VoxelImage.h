/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef VOXELIMAGE_H_
#define VOXELIMAGE_H_
#include "../_common.h"

#include "Asset.h"
#include "../Color.h"

namespace rheel {

struct RE_API _VoxelImageData {
	unsigned width;
	unsigned height;
	unsigned depth;
	std::vector<Color> voxels;
};

class RE_API VoxelImage : public Asset<_VoxelImageData> {

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
