/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_TEXTURE2DMULTISAMPLE_H
#define RHEELENGINE_TEXTURE2DMULTISAMPLE_H
#include "../../_common.h"

#include "Texture.h"
#include "Enums.h"

namespace rheel::gl {

class RE_API Texture2DMultisample : public Texture {

public:
	Texture2DMultisample();

	/**
	 * Initialize the data storage for this texture.
	 */
	void Initialize(InternalFormat internal_format, unsigned width, unsigned height, unsigned samples, bool fixed_sample_locations = true);

};

}

#endif
