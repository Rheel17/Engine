/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "GLVertexArray.h"

namespace rheel {

GLVertexArray::VertexAttribute::VertexAttribute(GLuint index, GLint size, GLenum type, GLsizei stride, GLsizeiptr offset, bool normalize) :
		_index(index), _size(size), _type(type), _stride(stride), _offset(offset), _normalize(normalize) {

	if (size < 1 && size > 4 && size != GL_RGBA) {
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

GLVertexArray::VertexAttribute::VertexAttribute() :
		_index(0), _size(0), _type(0), _stride(0), _offset(0), _normalize(false) {}

GLsizei GLVertexArray::VertexAttribute::_ByteSize() const {
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

GLVertexArray::GLVertexArray() {
	_id = GL::GenVertexArray();

	GLint maxVertexAttribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);

	for (GLint i = 0; i < maxVertexAttribs; i++) {
		_unused_attribute_indices.insert(i);
	}
}

GLuint GLVertexArray::ID() const {
	return _id;
}

void GLVertexArray::Bind() const {
	GL::BindVertexArray(_id);
}

void GLVertexArray::SetVertexAttributes(const GLBuffer& buffer, const std::vector<VertexAttribute>& attributes, bool instanced) {
	if (buffer.Target() != GL::BufferTarget::ARRAY) {
		throw std::invalid_argument("buffer must have target ARRAY");
	}

	Bind();

	// force that glBindBuffer() is actually called when this VAO is bound.
	GL::ClearBufferBinding(GL::BufferTarget::ARRAY);
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

void GLVertexArray::SetVertexIndices(const GLBuffer& buffer) {
	if (buffer.Target() != GL::BufferTarget::ELEMENT_ARRAY) {
		throw std::invalid_argument("buffer must have target ELEMENT_ARRAY");
	}

	Bind();

	// force that glBindBuffer() is actually called when this VAO is bound.
	GL::ClearBufferBinding(GL::BufferTarget::ELEMENT_ARRAY);
	buffer.Bind();
}

void GLVertexArray::_SetVertexAttributes(const GLBuffer& buffer, const std::vector<std::type_index>& attributeTypes, GLsizei stride, bool instanced) {
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

GLuint GLVertexArray::_FirstUnusedIndex(GLuint consecutive) const {
	GLuint count = 0;
	GLuint startIndex = 0;

	for (auto iter = _unused_attribute_indices.begin(); iter != _unused_attribute_indices.end(); iter++) {
		if (count == 0) {
			count = 1;
			startIndex = *iter;
		} else if (*iter == startIndex + count) {
			count++;
		} else {
			count = 1;
			startIndex = *iter;
		}

		if (count == consecutive) {
			return startIndex;
		}
	}

	throw std::runtime_error("No " + std::to_string(consecutive) + " consecutive indices available.");
}

}
