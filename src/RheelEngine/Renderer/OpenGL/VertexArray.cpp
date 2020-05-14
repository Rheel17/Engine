/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "VertexArray.h"

#include "Context.h"
#include "Capabilities.h"

namespace rheel::gl {

VertexArray::VertexAttribute::VertexAttribute(GLuint index, GLint size, Type type, GLsizei stride, GLsizeiptr offset, bool normalize) :
		_index(index),
		_size(size),
		_type(type),
		_stride(stride),
		_offset(offset),
		_normalize(normalize) {

	if ((size < 1 || size > 4) && size != GL_RGBA) {
		throw std::invalid_argument("size argument in VertexAttribute must be 1, 2, 3, 4, or GL_RGBA.");
	}

	if (type != Type::BYTE && type != Type::UNSIGNED_BYTE && type != Type::SHORT && type != Type::UNSIGNED_SHORT &&
			type != Type::INT && type != Type::UNSIGNED_INT && type != Type::HALF_FLOAT && type != Type::FLOAT &&
			type != Type::DOUBLE && type != Type::FIXED && type != Type::INT_2_10_10_10_REV &&
			type != Type::UNSIGNED_INT_2_10_10_10_REV && type != Type::UNSIGNED_INT_10F_11F_11F_REV) {
		throw std::invalid_argument("type argument in VertexAttribute is invalid.");
	}

	if (size == GL_RGBA && type != Type::UNSIGNED_BYTE && type != Type::INT_2_10_10_10_REV && type != Type::UNSIGNED_INT_2_10_10_10_REV) {
		throw std::invalid_argument("size GL_RGBA must be paired with Type::UNSIGNED_BYTE, Type::INT_2_10_10_10_REV, or Type::UNSIGNED_INT_2_10_10_10_REV.");
	}

	if ((type == Type::INT_2_10_10_10_REV || type == Type::UNSIGNED_INT_2_10_10_10_REV) && size != 4 && size != GL_RGBA) {
		throw std::invalid_argument("type Type::INT_2_10_10_10_REV or Type::UNSIGNED_INT_2_10_10_10_REV must be paired with size 4 or GL_RGBA.");
	}

	if (type == Type::UNSIGNED_INT_10F_11F_11F_REV && size != 3) {
		throw std::invalid_argument("type Type::UNSIGNED_INT_10F_11F_11F_REV must be paired with size 3.");
	}

	if (size == GL_RGBA && !normalize) {
		throw std::invalid_argument("size GL_RGBA must have normalize set to true.");
	}
}

VertexArray::VertexAttribute::VertexAttribute() :
		_index(0),
		_size(0),
		_type(),
		_stride(0),
		_offset(0),
		_normalize(false) {}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-branch-clone"
GLsizei VertexArray::VertexAttribute::ByteSize_() const {
	switch (_type) {
		case Type::BYTE:                         return _size;
		case Type::UNSIGNED_BYTE:                return _size == GL_RGBA ? 4 : _size;
		case Type::SHORT:                        return _size * 2;
		case Type::UNSIGNED_SHORT:               return _size * 2;
		case Type::INT:                          return _size * 4;
		case Type::UNSIGNED_INT:                 return _size * 4;
		case Type::HALF_FLOAT:                   return _size * 2;
		case Type::FLOAT:                        return _size * 4;
		case Type::DOUBLE:                       return _size * 8;
		case Type::FIXED:                        return _size * 4;
		case Type::INT_2_10_10_10_REV:           return 4;
		case Type::UNSIGNED_INT_2_10_10_10_REV:  return 4;
		case Type::UNSIGNED_INT_10F_11F_11F_REV: return 4;
		default:
			abort();
			return 0;
	}
}
#pragma clang diagnostic pop

void VertexArray::Bind() const {
	if (GetName() == 3) {
		std::cout << "";
	}

	Context::Current().BindVertexArray(*this);
}

void VertexArray::SetVertexAttributes(const Buffer& buffer, const std::vector<VertexAttribute>& attributes, unsigned instanceDivisor) {
	if (buffer.GetTarget() != Buffer::Target::ARRAY) {
		throw std::invalid_argument("buffer must have target ARRAY");
	}

	Bind();
	buffer.Bind();

	for (auto attribute : attributes) {
		glEnableVertexAttribArray(attribute._index);
		glVertexAttribPointer(
				attribute._index,
				attribute._size,
				GLenum(attribute._type),
				attribute._normalize,
				attribute._stride,
				(GLvoid*) attribute._offset);

		if (instanceDivisor > 0) {
			glVertexAttribDivisor(attribute._index, instanceDivisor);
		}
	}
}

void VertexArray::SetVertexIndices(const std::vector<GLubyte>& indices) {
	SetIndices_(indices, GL_UNSIGNED_BYTE);
}

void VertexArray::SetVertexIndices(const std::vector<GLushort>& indices) {
	SetIndices_(indices, GL_UNSIGNED_SHORT);
}

void VertexArray::SetVertexIndices(const std::vector<GLuint>& indices) {
	SetIndices_(indices, GL_UNSIGNED_INT);
}

void VertexArray::DrawArrays(Mode mode, int first, unsigned count, unsigned int instances) const {
	Bind();

	if (instances != 1) {
		glDrawArrays(GLenum(mode), first, count);
	} else {
		glDrawArraysInstanced(GLenum(mode), first, count, instances);
	}
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

void VertexArray::DrawElements(Mode mode, unsigned count, unsigned offset, unsigned instances) const {
	Bind();

	if (instances != 1) {
		glDrawElementsInstanced(GLenum(mode), count, _index_type, (const void*) (offset), instances);
	} else {
		glDrawElements(GLenum(mode), count, _index_type, (const void*) (offset));
	}
}

#pragma GCC diagnostic pop
#pragma clang diagnostic pop

void VertexArray::DrawElements(VertexArray::Mode mode, unsigned instances) const {
	DrawElements(mode, _index_count, 0, instances);
}

void VertexArray::DrawArraysIndirect(VertexArray::Mode mode, const DrawArraysIndirectBuffer& indirect, size_t count) const {
	Bind();
	indirect.Bind();

	if (count == 1) {
		glDrawArraysIndirect(GLenum(mode), nullptr);
	} else {
		glMultiDrawArraysIndirect(GLenum(mode), nullptr, count, 0);
	}
}

void VertexArray::DrawElementsIndirect(VertexArray::Mode mode, const DrawElementsIndirectBuffer& indirect, size_t count) const {
	Bind();
	indirect.Bind();

	if (count == 1) {
		glDrawElementsIndirect(GLenum(mode), _index_type, nullptr);
	} else {
		glMultiDrawElementsIndirect(GLenum(mode), _index_type, nullptr, count, 0);
	}
}

void VertexArray::SetVertexAttributes_(const Buffer& buffer, const std::vector<std::type_index>& attributeTypes, GLsizei stride, unsigned instanceDivisor) {
	std::vector<VertexAttribute> attributes;

	GLsizeiptr offset = 0;

	for (auto t : attributeTypes) {
		VertexAttribute attribute;
		GLuint index = FirstUnusedIndex_();

		if (t == typeid(float) || t == typeid(vec1)) {
			attribute = VertexAttribute(index, 1, Type::FLOAT, 0, offset);
		} else if (t == typeid(vec2)) {
			attribute = VertexAttribute(index, 2, Type::FLOAT, 0, offset);
		} else if (t == typeid(vec3)) {
			attribute = VertexAttribute(index, 3, Type::FLOAT, 0, offset);
		} else if (t == typeid(vec4) || t == typeid(quat)) {
			attribute = VertexAttribute(index, 4, Type::FLOAT, 0, offset);
		} else if (t == typeid(mat4)) {
			index = FirstUnusedIndex_(4);

			attribute = attributes.emplace_back(index++, 4, Type::FLOAT, 0, offset);
			_unused_attribute_indices.erase(attribute._index);

			attribute = attributes.emplace_back(index++, 4, Type::FLOAT, 0, offset + attribute.ByteSize_());
			_unused_attribute_indices.erase(attribute._index);

			attribute = attributes.emplace_back(index++, 4, Type::FLOAT, 0, offset + attribute.ByteSize_() * 2);
			_unused_attribute_indices.erase(attribute._index);

			attribute = VertexAttribute(index, 4, Type::FLOAT, 0, offset + attribute.ByteSize_() * 3);
			offset += attribute.ByteSize_() * 3;
		} else if (t == typeid(int) || t == typeid(ivec1)) {
			attribute = VertexAttribute(index, 1, Type::INT, 0, offset);
		} else if (t == typeid(ivec2)) {
			attribute = VertexAttribute(index, 2, Type::INT, 0, offset);
		} else if (t == typeid(ivec3)) {
			attribute = VertexAttribute(index, 3, Type::INT, 0, offset);
		} else if (t == typeid(ivec4)) {
			attribute = VertexAttribute(index, 4, Type::INT, 0, offset);
		} else {
			throw std::runtime_error("attribute type " + std::string(t.name()) + " not available.");
		}

		attributes.push_back(attribute);
		offset += attribute.ByteSize_();
		_unused_attribute_indices.erase(attribute._index);
	}

	GLsizei attributeStride = stride == 0 ? offset : stride;

	for (auto& attribute : attributes) {
		attribute._stride = attributeStride;
	}

	SetVertexAttributes(buffer, attributes, instanceDivisor);
}

GLuint VertexArray::FirstUnusedIndex_(GLuint consecutive) {
	if (!_has_initialized_unused_attribute_indices) {
		int maxVertexAttribs = Capabilities::GetMaxVertexAttribs();

		for (int i = 0; i < maxVertexAttribs; i++) {
			_unused_attribute_indices.insert(GLuint(i));
		}

		_has_initialized_unused_attribute_indices = true;
	}

	GLuint count = 0;
	GLuint startIndex = 0;

	for (unsigned int _unused_attribute_index : _unused_attribute_indices) {
		if (count != 0) {
			if (_unused_attribute_index == startIndex + count) {
				count++;
			} else {
				count = 1;
				startIndex = _unused_attribute_index;
			}
		} else {
			count = 1;
			startIndex = _unused_attribute_index;
		}

		if (count == consecutive) {
			return startIndex;
		}
	}

	throw std::runtime_error("No " + std::to_string(consecutive) + " consecutive indices available.");
}

}