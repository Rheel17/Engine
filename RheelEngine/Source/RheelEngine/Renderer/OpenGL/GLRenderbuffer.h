#ifndef GLRENDERBUFFER_H_
#define GLRENDERBUFFER_H_
#include "../../_common.h"

#include "GL.h"

namespace rheel {

class RE_API GLRenderbuffer {

public:
	/**
	 * Creates an OpenGL renderbuffer with the specified width, height, and internal format.
	 * The renderbuffer is automatically initialized using
	 * glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height).
	 *
	 * If samples is not set to 1, the renderbuffer is initialized using
	 * glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height).
	 */
	GLRenderbuffer(GLuint width, GLuint height, GLenum internalFormat, GLuint samples = 1);

	/**
	 * Returns the internal OpenGL ID of this renderbuffer.
	 */
	GLuint GetID() const;

	/**
	 * Binds this renderbuffer.
	 */
	void Bind() const;

	/**
	 * Clears the renderbuffer binding.
	 */
	static void ClearBinding();

private:
	GLhandle _id;

};

}

#endif
