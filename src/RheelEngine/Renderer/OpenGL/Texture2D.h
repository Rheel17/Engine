/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_TEXTURE2D_H
#define RHEELENGINE_GL_TEXTURE2D_H
#include "../../_common.h"

#include "Texture.h"
#include "Enums.h"

namespace rheel::GL {

class RE_API Texture2D : public Texture {

public:
	Texture2D();

	/**
	 * Sets the wrapping parameter for the horizontal direction.
	 */
	void SetWrapParameterS(WrapParameter parameter);

	/**
	 * Sets the wrapping parameter for the vertical direction.
	 */
	void SetWrapParameterT(WrapParameter parameter);

	/**
	 * Clears the texture and sets the size.
	 */
	void SetEmpty(InternalFormat internalFormat, unsigned width, unsigned height);

	/**
	 * Sets the data of this texture. A raw data pointer is used. Make sure the type and
	 * data type match up.
	 */
	void SetData(InternalFormat internalFormat, unsigned width, unsigned height, Format format, Type type, const void *data);

	/**
	 * Sets the data of this texture using the float type.
	 */
	void SetData(InternalFormat internalFormat, unsigned width, unsigned height, Format format, const float *data);

};

}

#endif
