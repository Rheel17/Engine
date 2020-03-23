/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef GLVERTEXARRAY_H_
#define GLVERTEXARRAY_H_
#include "../../_common.h"

#include <typeindex>
#include <set>

#include "_GLBuffer.h"

namespace rheel {

class RE_API _GLVertexArray {

public:
	class VertexAttribute {
		friend class _GLVertexArray;

	public:
		/**
		 * Specifies a vertex attribute in a Vertex Array Object.
		 *
		 * index:     the shader index of the attribute
		 * size:      the number of components per attribute. Must be 1, 2, 3, 4, or GL_RGBA.
		 * type:      the data type of each component. See
		 *            https://www.khronos.org/registry/_OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
		 *            for a list of accepted types.
		 * offset:    the offset in the buffer.
		 * normalize: when true, _OpenGL will normalize the data before they are accessed. (default: false)
		 */
		VertexAttribute(GLuint index, GLint size, GLenum type, GLsizei stride, GLsizeiptr offset, bool normalize = false);

	private:
		GLuint _index;
		GLint _size;
		GLenum _type;
		GLsizei _stride;
		GLsizeiptr _offset;
		bool _normalize;

		VertexAttribute();
		GLsizei _ByteSize() const;

	};

public:
	/**
	 * Creates an _OpenGL Vertex Array Object.
	 */
	_GLVertexArray();

	/**
	 * Returns the internal _OpenGL ID of this buffer.
	 */
	GLuint ID() const;

	/**
	 * Binds this buffer to the previously specified target.
	 */
	void Bind() const;

	/**
	 * Set the vertex attributes of this VAO. The specified buffer must have
	 * an ARRAY target. Use the attributes vector the specify actual vertex
	 * attributes.
	 */
	void SetVertexAttributes(const _GLBuffer& buffer, const std::vector<VertexAttribute>& attributes, bool instanced = false);

	/**
	 * Set the vertex attributes of this VAO. The specified buffer must have
	 * an ARRAY target. Use template arguments to automatically create
	 * VertexAttribute values.
	 *
	 * Example:
	 *
	 * SetVertexAttributes<vec2, float>(buffer);
	 *
	 * is equivalent to:
	 *
	 * SetVertexAttribtes(buffer, {
	 * 		VertexAttribute(0, 2, GL_FLOAT, 12, 0),
	 * 		VertexAttribute(1, 1, GL_FLOAT, 12, 8)
	 * });
	 *
	 * The attributes will always have consecutive indices starting at 0, and
	 * assumed to be tightly packed in the buffer. If this is not the case, use
	 * custom VertexAttribute values.
	 */
	template<typename... Types>
	void SetVertexAttributes(const _GLBuffer& buffer, GLsizei stride = 0, bool instanced = false) {
		_SetVertexAttributes(buffer, { typeid(Types)... }, stride, instanced);
	}

	/**
	 * Set the vertex indices of this VAO. The specified buffer must have an
	 * ELEMENT_ARRAY target.
	 */
	void SetVertexIndices(const _GLBuffer& buffer);

private:
	void _SetVertexAttributes(const _GLBuffer& buffer, const std::vector<std::type_index>& attributeTypes, GLsizei stride, bool instanced);
	GLuint _FirstUnusedIndex(GLuint consecutive = 1) const;

	GLhandle _id;
	std::set<GLuint> _unused_attribute_indices;

};

}

#endif
