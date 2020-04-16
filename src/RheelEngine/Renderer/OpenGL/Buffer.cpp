/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Buffer.h"

#include "State.h"

namespace rheel::gl {

Buffer::Buffer(gl::Buffer::Target target) :
		_target(target) {}

void Buffer::Bind() const {
	State::BindBuffer(*this);
}

Buffer::Target Buffer::GetTarget() const {
	return _target;
}

void Buffer::SetDataEmpty(Buffer::Usage usage) {
	glBufferData(GLenum(_target), 0, nullptr, GLenum(usage));
}

}