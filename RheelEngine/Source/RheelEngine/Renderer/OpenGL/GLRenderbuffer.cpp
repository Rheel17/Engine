#include "GLRenderbuffer.h"

namespace rheel {

GLRenderbuffer::GLRenderbuffer(GLuint width, GLuint height, GLenum internalFormat, GLuint samples) {
	_id = GL::GenRenderbuffer();
	Bind();

	if (samples == 1) {
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	} else {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
	}
}

GLuint GLRenderbuffer::GetID() const {
	return _id;
}

void GLRenderbuffer::Bind() const {
	GL::BindRenderbuffer(_id);
}

void GLRenderbuffer::ClearBinding() {
	GL::ClearRenderbufferBinding();
}

}
