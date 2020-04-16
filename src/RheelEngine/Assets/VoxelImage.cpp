/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "VoxelImage.h"

#include <fstream>
#include <sstream>

namespace rheel {

VoxelImage::VoxelImage(unsigned width, unsigned height, unsigned depth) :
		Asset({ width, height, depth, {} }) {

	GetRaw()->voxels.resize(width * height * depth);
}

VoxelImage::VoxelImage(unsigned width, unsigned height, unsigned depth, std::vector<Color> voxels) :
		Asset({ width, height, depth, std::move(voxels) }) {}

unsigned VoxelImage::GetWidth() const {
	return GetRaw()->width;
}

unsigned VoxelImage::GetHeight() const {
	return GetRaw()->height;
}

unsigned VoxelImage::GetDepth() const {
	return GetRaw()->depth;
}

const Color& VoxelImage::At(unsigned x, unsigned y, unsigned z) const {
	auto data = GetRaw();
	return data->voxels[x + y * data->width + z * data->width * data->height];
}

Color& VoxelImage::At(unsigned x, unsigned y, unsigned z) {
	auto data = GetRaw();
	return data->voxels[x + y * data->width + z * data->width * data->height];
}

}
