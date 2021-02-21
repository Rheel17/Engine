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

Buffer::AllocationPolicy Buffer::GetAllocationPolicy() const {
	return _allocation_policy;
}

void Buffer::SetAllocationPolicy(AllocationPolicy policy) {
	_allocation_policy = policy;
}

void Buffer::InvalidateData() {
	glInvalidateBufferData(GetName());
}

void Buffer::SetDataEmpty(Usage usage) {
	Bind();
	glBufferData(GLenum(_target), 0, nullptr, GLenum(usage));

	_byte_size = 0;
}

void Buffer::SetDataEmptySize(size_t byte_count, Buffer::Usage usage) {
	Bind();

	if (_allocation_policy == AllocationPolicy::REALLOCATE || byte_count > _byte_size) {
		glBufferData(GLenum(_target), byte_count, nullptr, GLenum(usage));
		_byte_size = byte_count;
	}
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
		case Buffer::Target::DRAW_INDIRECT:
			return out << "DRAW_INDIRECT";
	}

	return out;
}

}