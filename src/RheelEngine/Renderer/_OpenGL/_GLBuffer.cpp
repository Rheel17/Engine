/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "_GLBuffer.h"

namespace rheel {

_GLBuffer::_GLBuffer(_GL::BufferTarget target)
		: _target(target) {

	_id = _GL::GenBuffer();
}

GLuint _GLBuffer::ID() const {
	return _id;
}

_GL::BufferTarget _GLBuffer::Target() const {
	return _target;
}

void _GLBuffer::Bind() const {
	_GL::BindBuffer(_id, _target);
}

void _GLBuffer::ClearBinding(_GL::BufferTarget target, bool force) {
	_GL::ClearBufferBinding(target, force);
}

void _GLBuffer::SetData(const void *data, GLsizeiptr size, Usage usage) {
	Bind();
	glBufferData(_GL::Target(_target), size, data, usage);
}

}
