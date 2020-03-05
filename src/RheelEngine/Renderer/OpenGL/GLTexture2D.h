/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef GLTEXTURE2D_H_
#define GLTEXTURE2D_H_
#include "../../_common.h"

#include "GL.h"

namespace rheel {

class RE_API GLTexture2D {

public:
	GLTexture2D(GLuint width, GLuint height, GLuint internalFormat);

	/**
	 * Returns the internal OpenGL ID of this texture.
	 */
	GLuint ID() const;

	/**
	 * Binds this texture to the specified texture unit.
	 */
	void Bind(GLuint textureUnit = 0) const;

	/**
	 * Sets the minifying function for this texture.
	 */
	void SetMinifyingFilter(GL::FilterFunction filterFunction);

	/**
	 * Sets the magnification function for this texture.
	 */
	void SetMagnificationFilter(GL::FilterFunction filterFuntion);

	/**
	 * Sets the wrapping parameter for the horizontal direction.
	 */
	void SetWrapParameterS(GL::WrapParameter parameter);

	/**
	 * Sets the wrapping parameter for the vertical direction.
	 */
	void SetWrapParameterT(GL::WrapParameter parameter);

	/**
	 * Generates the mipmap for this texture
	 */
	void GenerateMipmap();

	/**
	 * Initializes this texture with empty data. All-zero bytes will be written
	 * to the complete texture.
	 */
	void InitializeEmpty(GLenum format);

	/**
	 * Sets the data of this texture.
	 *
	 * format: Specifies the format of the pixel data.
	 * type: Specifies the data type of the pixel data.
	 */
	void SetData(GLenum format, GLenum type, const void *data);

	/**
	 * Sets part of the data of this texture. The area enclosed by the
	 * (x, y, width, height) rectangle will be set with the data provided.
	 */
	void SetPartialData(int x, int y, unsigned width, unsigned height, GLenum format, GLenum type, const void *data);

private:
	GLhandle _id;

	GLuint _width;
	GLuint _height;
	GLuint _internal_format;

	bool _has_initialized = false;

};

}

#endif
