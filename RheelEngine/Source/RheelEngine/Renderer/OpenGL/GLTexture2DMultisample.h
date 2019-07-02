#ifndef GLTEXTURE2DMULTISAMPLE_H_
#define GLTEXTURE2DMULTISAMPLE_H_
#include "../../_common.h"

#include "GLTexture2D.h"

namespace rheel {

class RE_API GLTexture2DMultisample {

public:
	GLTexture2DMultisample(GLuint width, GLuint height, GLuint samples);

	/**
	 * Returns the internal OpenGL ID of this texture.
	 */
	GLuint GetID() const;

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
