/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef GLTEXTURE2DMULTISAMPLE_H_
#define GLTEXTURE2DMULTISAMPLE_H_
#include "../../_common.h"

#include "_GL.h"

namespace rheel {

class RE_API _GLTexture2DMultisample {

public:
	_GLTexture2DMultisample(GLuint width, GLuint height, GLuint samples);

	/**
	 * Returns the internal _OpenGL ID of this texture.
	 */
	GLuint ID() const;

	/**
	 * Binds this texture to the specified texture unit.
	 */
	void Bind(GLuint textureUnit = 0) const;

	/**
	 * Sets the data of this texture.
	 *
	 * internalFormat: Specifies the number of color components in the texture.
	 */
	void Initialize(GLint internalFormat);

private:
	GLhandle _id;

	GLuint _width;
	GLuint _height;
	GLuint _samples;

};

}

#endif
