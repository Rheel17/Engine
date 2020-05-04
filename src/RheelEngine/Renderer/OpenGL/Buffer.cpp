/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Buffer.h"

#include "Context.h"

namespace rheel::gl {

Buffer::Buffer(gl::Buffer::Target target) :
		_target(target) {}

void Buffer::Bind() const {
	Context::Current().BindBuffer(*this);
}

Buffer::Target Buffer::GetTarget() const {
	return _target;
}

void Buffer::SetDataEmpty(Buffer::Usage usage) {
	Bind();
	glBufferData(GLenum(_target), 0, nullptr, GLenum(usage));
}

std::ostream& operator<<(std::ostream& out, Buffer::Target target) {
	switch (target) {
		case Buffer::Target::ARRAY:
			return out << "ARRAY";
		case Buffer::Target::ATOMIC_COUNTER:
			return out << "ATOMIC_COUNTER";
		case Buffer::Target::COPY_READ:
			return out << "COPY_READ";
		case Buffer::Target::COPY_WRITE:
			return out << "COPY_WRITE";
		case Buffer::Target::DISPATCH_INDIRECT:
			return out << "DISPATCH_INDIRECT";
		case Buffer::Target::PIXEL_PACK:
			return out << "PIXEL_PACK";
		case Buffer::Target::PIXEL_UNPACK:
			return out << "PIXEL_UNPACK";
		case Buffer::Target::QUERY:
			return out << "QUERY";
		case Buffer::Target::SHADER_STORAGE:
			return out << "SHADER_STORAGE";
		case Buffer::Target::TEXTURE:
			return out << "TEXTURE";
		case Buffer::Target::TRANSFORM_FEEDBACK:
			return out << "TRANSFORM_FEEDBACK";
		case Buffer::Target::UNIFORM:
			return out << "UNIFORM";
	}

	return out;
}

}