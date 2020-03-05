/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_ASSETLOADER_H
#define RHEELENGINE_ASSETLOADER_H
#include "../_common.h"

#include "Loaders/ColladaLoader.h"
#include "Loaders/PngLoader.h"
#include "Loaders/VoxelLoader.h"
#include "Loaders/GlslLoader.h"

namespace rheel {

class RE_API AssetLoader {
	friend class Engine;

public:
	/**
	 * Loader for collada (.dae) model files
	 */
	const ColladaLoader collada = ColladaLoader();

	/**
	 * Loader for PNG (.png) image files
	 */
	const PngLoader png = PngLoader();

	/**
	 * Loader for voxel (.vox) voxel image files
	 */
	const VoxelLoader voxel = VoxelLoader();

	/**
	 * Loader for glsl (.glsl) shader source files
	 */
	const GlslLoader glsl = GlslLoader();

private:
	AssetLoader() = default;

};

}

#endif
