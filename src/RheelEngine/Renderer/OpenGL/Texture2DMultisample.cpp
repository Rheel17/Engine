/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Texture2DMultisample.h"

namespace rheel::gl {

Texture2DMultisample::Texture2DMultisample() :
		Texture(Target::TEXTURE_2D_MULTISAMPLE) {}

void Texture2DMultisample::Initialize(InternalFormat internalFormat, unsigned width, unsigned height, unsigned samples, bool fixedSampleLocations) {
	Bind();
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GLenum(internalFormat), width, height, fixedSampleLocations);
}

}