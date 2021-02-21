/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_ABSTRACTTEXTURE_H
#define RHEELENGINE_ABSTRACTTEXTURE_H
#include "../../_common.h"

#include "Texture.h"

namespace rheel::gl {

class RE_API AbstractTexture : public Texture {

public:
	explicit AbstractTexture(Texture::Target target);

	/**
	 * Sets the minifying function for this texture.
	 */
	void SetMinifyingFilter(FilterFunction filter_function);

	/**
	 * Sets the magnification function for this texture.
	 */
	void SetMagnificationFilter(FilterFunction filter_function);

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
