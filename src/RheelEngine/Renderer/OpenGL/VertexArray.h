/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_VERTEXARRAY_H
#define RHEELENGINE_GL_VERTEXARRAY_H
#include "../../_common.h"

#include <typeindex>
#include <set>

#include "Buffer.h"
#include "Enums.h"

namespace rheel::GL {

OPENGL_GEN_FUNCTION(glGenVertexArrays, _GenVertexArrays);
OPENGL_DELETE_FUNCTION(glDeleteVertexArrays, _DeleteVertexArrays);

class RE_API VertexArray : public Object<_GenVertexArrays, _DeleteVertexArrays> {


public:
	class VertexAttribute {
		friend class VertexArray;

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
		VertexAttribute(GLuint index, GLint size, Type type, GLsizei stride, GLsizeiptr offset, bool normalize = false);

	private:
		GLuint _index;
		GLint _size;
		Type _type;
		GLsizei _stride;
		GLsizeiptr _offset;
		bool _normalize;

		VertexAttribute();
		GLsizei _ByteSize() const;

	};

	enum class Mode {
		POINTS = GL_POINTS,
		LINE_STRIP = GL_LINE_STRIP,
		LINE_LOOP = GL_LINE_LOOP,
		LINES = GL_LINES,
		LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
		LINES_ADJACENCY = GL_LINES_ADJACENCY,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		TRIANGLE_FAN = GL_TRIANGLE_FAN,
		TRIANGLES = GL_TRIANGLES,
		TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
		TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
		PATCHES = GL_PATCHES
	};

	enum class IndexType {
		UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
		UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
		UNSIGNED_INT = GL_UNSIGNED_INT
	};

public:
	void Bind() const;

	/**
	 * Set the vertex attributes of this VAO. The specified buffer must have
	 * an ARRAY target. Use the attributes vector the specify actual vertex
	 * attributes.
	 */
	void SetVertexAttributes(const Buffer& buffer, const std::vector<VertexAttribute>& attributes, bool instanced = false);

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
	void SetVertexAttributes(const Buffer& buffer, GLsizei stride = 0, bool instanced = false) {
		_SetVertexAttributes(buffer, { typeid(Types)... }, stride, instanced);
	}

	/**
	 * Set the vertex indices of this VAO. The specified buffer must have an
	 * ELEMENT_ARRAY target.
	 */
	void SetVertexIndices(const Buffer& buffer, IndexType indexType);

	/**
	 * Draws the contents of the VAO using glDrawArrays or glDrawArraysInstanced,
	 * depending on the number of indices. Make sure that the VAO is complete, i.e.
	 * it has vertex data. The index buffer will not be used for this draw call, use
	 * DrawElements(...) for that.
	 */
	void DrawArrays(Mode mode, int first, unsigned count, unsigned instances = 1) const;

	/**
	 * Draws the contents of the VAO using glDrawElements or glDrawElementsInstanced,
	 * depending on the number of indices. Make sure that the VAO is complete, i.e.
	 * it has vertex data and indices.
	 */
	void DrawElements(Mode mode, unsigned count, unsigned offset = 0, unsigned instances = 1) const;

private:
	void _SetVertexAttributes(const Buffer& buffer, const std::vector<std::type_index>& attributeTypes, GLsizei stride, bool instanced);
	GLuint _FirstUnusedIndex(GLuint consecutive = 1);

	IndexType _index_type{};

	bool _has_initialized_unused_attribute_indices = false;
	std::set<GLuint> _unused_attribute_indices;

};

}

#endif
