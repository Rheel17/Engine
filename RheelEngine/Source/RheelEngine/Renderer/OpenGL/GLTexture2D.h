#ifndef GLTEXTURE2D_H_
#define GLTEXTURE2D_H_
#include "../../_common.h"

#include "GL.h"

namespace rheel {

class RE_API GLTexture2D {

public:
	GLTexture2D(GLuint width, GLuint height);

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
	 * format: Specifies the format of the pixel data.
	 * type: Specifies the data type of the pixel data.
	 */
	void SetData(GLint internalFormat, GLenum format, GLenum type, const void *data);

private:
	GLhandle _id;

	GLuint _width;
	GLuint _height;

};

}

#endif
