/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Buffer.h"

#include "State.h"

namespace rheel::GL {

Buffer::Buffer(GL::Buffer::Target target) :
		_target(target) {}

void Buffer::Bind() const {
	State::BindBuffer(*this);
}

Buffer::Target Buffer::GetTarget() const {
	return _target;
}

void Buffer::SetData(const void *data, ptrdiff_t size, Buffer::Usage usage) {
	Bind();
	glBufferData(GLenum(_target), size, data, GLenum(usage));
}

}