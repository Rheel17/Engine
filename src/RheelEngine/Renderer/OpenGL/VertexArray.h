/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_VERTEXARRAY_H
#define RHEELENGINE_VERTEXARRAY_H
#include "../../_common.h"

#include <typeindex>
#include <set>

#include "Buffer.h"
#include "Enums.h"

namespace rheel::gl {

OPENGL_GEN_FUNCTION(glGenVertexArrays, gen_vertex_arrays_);
OPENGL_DELETE_FUNCTION(glDeleteVertexArrays, delete_vertex_arrays_);

class RE_API VertexArray : public Object<gen_vertex_arrays_, delete_vertex_arrays_> {

private:
	class ElementArrayBuffer : public Object<gen_buffers_, delete_buffers_> {};

public:
	class VertexAttribute {
		friend class VertexArray;

	public:
		/**
		 * Specifies a vertex attribute in a Vertex Array Object.
		 *
		 * index:     the shader index of the attribute
		 * size:      the number of components per attribute. Must be 1, 2, 3,
		 *            4, or GL_RGBA.
		 * type:      the data type of each component. See
		 *            https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
		 *            for a list of accepted types.
		 * offset:    the offset in the buffer.
		 * normalize: when true, OpenGL will normalize the data before they are
		 *            accessed. (default: false)
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
		GLsizei ByteSize_() const;

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

public:
	void Bind() const;

	/**
	 * Set the vertex attributes of this VAO. The specified buffer must have an
	 * ARRAY target. Use the attributes vector the specify actual vertex
	 * attributes.
	 */
	void SetVertexAttributes(const Buffer& buffer, const std::vector<VertexAttribute>& attributes, bool instanced = false);

	/**
	 * Set the vertex attributes of this VAO. The specified buffer must have an
	 * ARRAY target. Use template arguments to automatically create
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
		SetVertexAttributes_(buffer, { typeid(Types)... }, stride, instanced);
	}

	/**
	 * Set the vertex indices of this VAO.
	 */
	void SetVertexIndices(const std::vector<GLubyte>& indices);

	/**
	 * Set the vertex indices of this VAO.
	 */
	void SetVertexIndices(const std::vector<GLushort>& indices);

	/**
	 * Set the vertex indices of this VAO.
	 */
	void SetVertexIndices(const std::vector<GLuint>& indices);

	/**
	 * Draws the contents of the VAO using glDrawArrays or
	 * glDrawArraysInstanced, depending on the number of indices. Make sure that
	 * the VAO is complete, i.e. it has vertex data. The index buffer will not
	 * be used for this draw call, use DrawElements(...) for that.
	 */
	void DrawArrays(Mode mode, int first, unsigned count, unsigned instances = 1) const;

	/**
	 * Draws the contents of the VAO using glDrawElements or
	 * glDrawElementsInstanced, depending on the number of indices. Make sure
	 * that the VAO is complete, i.e. it has vertex data and indices.
	 */
	void DrawElements(Mode mode, unsigned count, unsigned offset, unsigned instances = 1) const;

	/**
	 * Draws the contents of the VAO using glDrawElements or
	 * glDrawElementsInstanced, depending on the number of indices. Make sure
	 * that the VAO is complete, i.e. it has vertex data and indices.
	 */
	void DrawElements(Mode mode, unsigned instances = 1) const;

	/**
	 * Draws the contents ofthe VAO using glDrawElementsIndirect, using the
	 * indirect buffer as GL_DRAW_INDRECT_BUFFER. Make sure that the indirect
	 * buffer's target is DRAW_INDRECT.
	 */
	void MultiDrawElementsIndirect(VertexArray::Mode mode, const Buffer& indirect, size_t count) const;

private:
	template<typename T>
	void SetIndices_(const std::vector<T>& indices, GLenum type) {
		Bind();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer.GetName());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(T), indices.data(), GL_STATIC_DRAW);

		_index_type = type;
		_index_count = indices.size();
	}

	void SetVertexAttributes_(const Buffer& buffer, const std::vector<std::type_index>& attributeTypes, GLsizei stride, bool instanced);
	GLuint FirstUnusedIndex_(GLuint consecutive = 1);

	ElementArrayBuffer _index_buffer;
	unsigned _index_count;
	GLenum _index_type;

	bool _has_initialized_unused_attribute_indices = false;
	std::set<GLuint> _unused_attribute_indices;

};

}

#endif
