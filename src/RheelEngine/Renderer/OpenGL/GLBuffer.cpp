/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "GLBuffer.h"

namespace rheel {

GLBuffer::GLBuffer(GL::BufferTarget target)
		: _target(target) {

	_id = GL::GenBuffer();
}

GLuint GLBuffer::ID() const {
	return _id;
}

GL::BufferTarget GLBuffer::Target() const {
	return _target;
}

void GLBuffer::Bind() const {
	GL::BindBuffer(_id, _target);
}

void GLBuffer::ClearBinding(GL::BufferTarget target, bool force) {
	GL::ClearBufferBinding(target, force);
}

void GLBuffer::SetData(const void *data, GLsizeiptr size, Usage usage) {
	Bind();
	glBufferData(GL::Target(_target), size, data, usage);
}

}
