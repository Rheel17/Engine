/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_ABSTRACTTEXTURE_H
#define RHEELENGINE_ABSTRACTTEXTURE_H
#include "../../_common.h"

#include "Texture.h"

namespace rheel::GL {

class RE_API AbstractTexture : public Texture {

public:
	explicit AbstractTexture(Texture::Target target);

	/**
	 * Sets the minifying function for this texture.
	 */
	void SetMinifyingFilter(FilterFunction filterFunction);

	/**
	 * Sets the magnification function for this texture.
	 */
	void SetMagnificationFilter(FilterFunction filterFunction);

	/**
	 * Sets the anisotropy parameter
	 */
	void SetAnisotropyParameter(float parameter);

	/**
	 * Generates the mipmap for this texture
	 */
	void GenerateMipmap();

};

}

#endif
