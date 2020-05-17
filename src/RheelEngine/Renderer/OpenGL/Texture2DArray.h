/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_TEXTURE2DARRAY_H
#define RHEELENGINE_TEXTURE2DARRAY_H
#include "../../_common.h"

#include "AbstractTexture.h"
#include "Enums.h"

namespace rheel::gl {

class RE_API Texture2DArray : public AbstractTexture {

public:
	Texture2DArray();

	/**
	 * Sets the wrapping parameter for the horizontal direction.
	 */
	void SetWrapParameterS(WrapParameter parameter);

	/**
	 * Sets the wrapping parameter for the vertical direction.
	 */
	void SetWrapParameterT(WrapParameter parameter);

	/**
	 * Initializes the storage of this texture. Must be called before
	 * SetLayerData(...).
	 */
	void Initialize(InternalFormat internalFormat, unsigned width, unsigned height, unsigned layers);

	/**
	 * Sets the data of this texture. A raw data pointer is used. Make sure the
	 * type and data type match up.
	 */
	void SetData(InternalFormat internalFormat, unsigned width, unsigned height, unsigned layers, Format format, Type type, const void* data);

	/**
	 * Sets the data of this texture using the float type.
	 */
	void SetData(InternalFormat internalFormat, unsigned width, unsigned height, unsigned layers, Format format, const float* data);

	/**
	 * Sets a single layer of this texture. A raw data pointer is used. Make
	 * sure the type and data type match up. Also make sure that Initialize()
	 * was called on this texture before calling this.
	 */
	void SetLayerData(unsigned width, unsigned height, unsigned layer, Format format, Type type, const void* data);

	/**
 	 * Sets a single layer of this texture using the float type. Make sure that
	 * Initialize() was called on this texture before calling this.
 	*/
	void SetLayerData(unsigned width, unsigned height, unsigned layer, Format format, const float* data);

};

}

#endif
