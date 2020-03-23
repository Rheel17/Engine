/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "VertexArray.h"

#include "State.h"
#include "Capabilities.h"

namespace rheel::GL {

VertexArray::VertexAttribute::VertexAttribute(GLuint index, GLint size, GLenum type, GLsizei stride, GLsizeiptr offset, bool normalize) :
		_index(index), _size(size), _type(type), _stride(stride), _offset(offset), _normalize(normalize) {

	if ((size < 1 || size > 4) && size != GL_RGBA) {
		throw std::invalid_argument("size argument in VertexAttribute must be 1, 2, 3, 4, or GL_RGBA.");
	}

	if (type != GL_BYTE && type != GL_UNSIGNED_BYTE && type != GL_SHORT && type != GL_UNSIGNED_SHORT &&
		type != GL_INT && type != GL_UNSIGNED_INT && type != GL_HALF_FLOAT && type != GL_FLOAT &&
		type != GL_DOUBLE && type != GL_FIXED && type != GL_INT_2_10_10_10_REV &&
		type != GL_UNSIGNED_INT_2_10_10_10_REV && type != GL_UNSIGNED_INT_10F_11F_11F_REV) {
		throw std::invalid_argument("type argument in VertexAttribute is invalid.");
	}

	if (size == GL_RGBA && type != GL_UNSIGNED_BYTE && type != GL_INT_2_10_10_10_REV && type != GL_UNSIGNED_INT_2_10_10_10_REV) {
		throw std::invalid_argument("size GL_RGBA must be paired with GL_UNSIGNED_BYTE, GL_INT_2_10_10_10_REV, or GL_UNSIGNED_INT_2_10_10_10_REV.");
	}

	if ((type == GL_INT_2_10_10_10_REV || type == GL_UNSIGNED_INT_2_10_10_10_REV) && size != 4 && size != GL_RGBA) {
		throw std::invalid_argument("type GL_INT_2_10_10_10_REV or GL_UNSIGNED_INT_2_10_10_10_REV must be paired with size 4 or GL_RGBA.");
	}

	if (type == GL_UNSIGNED_INT_10F_11F_11F_REV && size != 3) {
		throw std::invalid_argument("type GL_UNSIGNED_INT_10F_11F_11F_REV must be paired with size 3.");
	}

	if (size == GL_RGBA && !normalize) {
		throw std::invalid_argument("size GL_RGBA must have normalize set to true.");
	}
}

VertexArray::VertexAttribute::VertexAttribute() :
		_index(0), _size(0), _type(0), _stride(0), _offset(0), _normalize(false) {}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-branch-clone"
GLsizei VertexArray::VertexAttribute::_ByteSize() const {
	switch (_type) {
		case GL_BYTE: 							return _size;
		case GL_UNSIGNED_BYTE: 					return _size == GL_RGBA ? 4 : _size;
		case GL_SHORT: 							return _size * 2;
		case GL_UNSIGNED_SHORT: 				return _size * 2;
		case GL_INT: 							return _size * 4;
		case GL_UNSIGNED_INT: 					return _size * 4;
		case GL_HALF_FLOAT: 					return _size * 2;
		case GL_FLOAT: 							return _size * 4;
		case GL_DOUBLE: 						return _size * 8;
		case GL_FIXED: 							return _size * 4;
		case GL_INT_2_10_10_10_REV: 			return 4;
		case GL_UNSIGNED_INT_2_10_10_10_REV:	return 4;
		case GL_UNSIGNED_INT_10F_11F_11F_REV:	return 4;
		default:
			assert(false);
			return 0;
	}
}
#pragma clang diagnostic pop

VertexArray::VertexArray() {
	int maxVertexAttribs = Capabilities::GetMaxVertexAttribs();

	for (int i = 0; i < maxVertexAttribs; i++) {
		_unused_attribute_indices.insert(GLuint(i));
	}
}

void VertexArray::Bind() const {
	State::BindVertexArray(*this);
}

void VertexArray::SetVertexAttributes(const Buffer& buffer, const std::vector<VertexAttribute>& attributes, bool instanced) {
	if (buffer.GetTarget() != Buffer::Target::ARRAY) {
		throw std::invalid_argument("buffer must have target ARRAY");
	}

	Bind();

	// TODO: bind the buffer
	buffer.Bind();

	for (auto attribute : attributes) {
		glEnableVertexAttribArray(attribute._index);
		glVertexAttribPointer(
		attribute._index,
		attribute._size,
		attribute._type,
		attribute._normalize,
		attribute._stride,
		(GLvoid *) attribute._offset);

		if (instanced) {
			glVertexAttribDivisor(attribute._index, 1);
		}
	}
}

void VertexArray::SetVertexIndices(const Buffer& buffer) {
	if (buffer.GetTarget() != Buffer::Target::ELEMENT_ARRAY) {
		throw std::invalid_argument("buffer must have target ELEMENT_ARRAY");
	}

	Bind();

	// TODO: bind the buffer
	buffer.Bind();
}

void VertexArray::_SetVertexAttributes(const Buffer& buffer, const std::vector<std::type_index>& attributeTypes, GLsizei stride, bool instanced) {
	std::vector<VertexAttribute> attributes;

	GLsizeiptr offset = 0;

	for (auto t : attributeTypes) {
		VertexAttribute attribute;
		GLuint index = _FirstUnusedIndex();

		if (t == typeid(float) || t == typeid(vec1)) {
			attribute = VertexAttribute(index, 1, GL_FLOAT, 0, offset);
		} else if (t == typeid(vec2)) {
			attribute = VertexAttribute(index, 2, GL_FLOAT, 0, offset);
		} else if (t == typeid(vec3)) {
			attribute = VertexAttribute(index, 3, GL_FLOAT, 0, offset);
		} else if (t == typeid(vec4) || t == typeid(quat)) {
			attribute = VertexAttribute(index, 4, GL_FLOAT, 0, offset);
		} else if (t == typeid(mat4)) {
			index = _FirstUnusedIndex(4);

			attribute = attributes.emplace_back(index++, 4, GL_FLOAT, 0, offset);
			_unused_attribute_indices.erase(attribute._index);

			attribute = attributes.emplace_back(index++, 4, GL_FLOAT, 0, offset + attribute._ByteSize());
			_unused_attribute_indices.erase(attribute._index);

			attribute = attributes.emplace_back(index++, 4, GL_FLOAT, 0, offset + attribute._ByteSize() * 2);
			_unused_attribute_indices.erase(attribute._index);

			attribute = VertexAttribute(index, 4, GL_FLOAT, 0, offset + attribute._ByteSize() * 3);
			offset += attribute._ByteSize() * 3;
		} else if (t == typeid(int) || t == typeid(ivec1)) {
			attribute = VertexAttribute(index, 1, GL_INT, 0, offset);
		} else if (t == typeid(ivec2)) {
			attribute = VertexAttribute(index, 2, GL_INT, 0, offset);
		} else if (t == typeid(ivec3)) {
			attribute = VertexAttribute(index, 3, GL_INT, 0, offset);
		} else if (t == typeid(ivec4)) {
			attribute = VertexAttribute(index, 4, GL_INT, 0, offset);
		} else {
			throw std::runtime_error("attribute type " + std::string(t.name()) + " not available.");
		}

		attributes.push_back(attribute);
		offset += attribute._ByteSize();
		_unused_attribute_indices.erase(attribute._index);
	}

	GLsizei attributeStride = stride == 0 ? offset : stride;

	for (auto& attribute : attributes) {
		attribute._stride = attributeStride;
	}

	SetVertexAttributes(buffer, attributes, instanced);
}

GLuint VertexArray::_FirstUnusedIndex(GLuint consecutive) const {
	GLuint count = 0;
	GLuint startIndex = 0;

	for (unsigned int _unused_attribute_indice : _unused_attribute_indices) {
		if (count == 0) {
			count = 1;
			startIndex = _unused_attribute_indice;
		} else if (_unused_attribute_indice == startIndex + count) {
			count++;
		} else {
			count = 1;
			startIndex = _unused_attribute_indice;
		}

		if (count == consecutive) {
			return startIndex;
		}
	}

	throw std::runtime_error("No " + std::to_string(consecutive) + " consecutive indices available.");
}

}