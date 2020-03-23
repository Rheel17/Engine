/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "_GLTexture2DMultisample.h"

namespace rheel {

_GLTexture2DMultisample::_GLTexture2DMultisample(GLuint width, GLuint height, GLuint samples) :
		_width(width), _height(height), _samples(samples) {

	_id = _GL::GenTexture();
}

GLuint _GLTexture2DMultisample::ID() const {
	return _id;
}

void _GLTexture2DMultisample::Bind(GLuint textureUnit) const {
	_GL::BindTexture(_id, _GL::TextureTarget::TEXTURE_2D_MULTISAMPLE, textureUnit);
}

void _GLTexture2DMultisample::Initialize(GLint internalFormat) {
	Bind();
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, internalFormat, _width, _height, true);
}

}
