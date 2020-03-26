/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_TEXTURE2DMULTISAMPLE_H
#define RHEELENGINE_TEXTURE2DMULTISAMPLE_H
#include "../../_common.h"

#include "Texture.h"
#include "Enums.h"

namespace rheel::GL {

class RE_API Texture2DMultisample : public Texture {

public:
	Texture2DMultisample();

	/**
	 * Sets the wrapping parameter for the horizontal direction.
	 */
	void SetWrapParameterS(WrapParameter parameter);

	/**
	 * Sets the wrapping parameter for the vertical direction.
	 */
	void SetWrapParameterT(WrapParameter parameter);

	/**
	 * Initialize the data storage for this texture.
	 */
	void Initialize(InternalFormat internalFormat, unsigned width, unsigned height, unsigned samples, bool fixedSampleLocations = true);

};

}

#endif
