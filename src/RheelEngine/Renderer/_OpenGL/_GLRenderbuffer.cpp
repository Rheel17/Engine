/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "_GLRenderbuffer.h"

namespace rheel {

_GLRenderbuffer::_GLRenderbuffer(GLuint width, GLuint height, GLenum internalFormat,
								 GLuint samples, bool forceMultisampled) {
	_id = _GL::GenRenderbuffer();
	Bind();

	if (samples == 1 && !forceMultisampled) {
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	} else {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
	}
}

GLuint _GLRenderbuffer::ID() const {
	return _id;
}

void _GLRenderbuffer::Bind() const {
	_GL::BindRenderbuffer(_id);
}

void _GLRenderbuffer::ClearBinding() {
	_GL::ClearRenderbufferBinding();
}

}
