/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_TEXTURE2D_H
#define RHEELENGINE_TEXTURE2D_H
#include "../../_common.h"

#include "AbstractTexture.h"
#include "Enums.h"

namespace rheel::gl {

class RE_API Texture2D : public AbstractTexture {

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
	 *Specifies the comparison operator used. Only usefull with
	 * SetCompareMode(COMPARE_REF_TO_TEXTURE).
	 */
	void SetCompareFunction(CompareFunction parameter);

	/**
	 * Specifies the texture comparison mode.
	 */
	void SetCompareMode(CompareMode parameter);

	/**
	 * Clears the texture and sets the size.
	 */
	void SetEmpty(InternalFormat internalFormat, unsigned width, unsigned height, Format format);

	/**
	 * Sets the data of this texture. A raw data pointer is used. Make sure the type and
	 * data type match up.
	 */
	void SetData(InternalFormat internalFormat, unsigned width, unsigned height, Format format, Type type, const void* data);

	/**
	 * Sets the data of this texture using the float type.
	 */
	void SetData(InternalFormat internalFormat, unsigned width, unsigned height, Format format, const float* data);

};

}

#endif
