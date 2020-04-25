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
	Loader<Model, ColladaLoader> collada = Loader<Model, ColladaLoader>();

	/**
	 * Loader for PNG (.png) image files
	 */
	Loader<Image, PngLoader> png = Loader<Image, PngLoader>();

	/**
	 * Loader for voxel (.vox) voxel image files
	 */
	Loader<VoxelImage, VoxelLoader> voxel = Loader<VoxelImage, VoxelLoader>();

	/**
	 * Loader for glsl (.glsl) shader source files
	 */
	Loader<Shader, GlslLoader> glsl = Loader<Shader, GlslLoader>();

private:
	AssetLoader() = default;

};

}

#endif
