/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_BUFFER_H
#define RHEELENGINE_BUFFER_H
#include "../../_common.h"

#include "Object.h"

OPENGL_GEN_FUNCTION(glGenBuffers, gen_buffers_);
OPENGL_DELETE_FUNCTION(glDeleteBuffers, delete_buffers_);

namespace rheel::gl {

class RE_API Buffer : public Object<gen_buffers_, delete_buffers_> {

public:
	enum class Target {
		ARRAY = GL_ARRAY_BUFFER,
		ATOMIC_COUNTER = GL_ATOMIC_COUNTER_BUFFER,
		COPY_READ = GL_COPY_READ_BUFFER,
		COPY_WRITE = GL_COPY_WRITE_BUFFER,
		DISPATCH_INDIRECT = GL_DISPATCH_INDIRECT_BUFFER,
		// GL_ELEMENT_ARRAY_BUFFER is not valid without a VAO context,
		// so putting it here would only encourage bugs
		PIXEL_PACK = GL_PIXEL_PACK_BUFFER,
		PIXEL_UNPACK = GL_PIXEL_UNPACK_BUFFER,
		QUERY = GL_QUERY_BUFFER,
		SHADER_STORAGE = GL_SHADER_STORAGE_BUFFER,
		TEXTURE = GL_TEXTURE_BUFFER,
		TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER,
		UNIFORM = GL_UNIFORM_BUFFER
	};

	enum class Usage {
		STREAM_DRAW = GL_STREAM_DRAW,
		STREAM_READ = GL_STREAM_READ,
		STREAM_COPY = GL_STREAM_COPY,
		STATIC_DRAW = GL_STATIC_DRAW,
		STATIC_READ = GL_STATIC_READ,
		STATIC_COPY = GL_STATIC_COPY,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		DYNAMIC_READ = GL_DYNAMIC_READ,
		DYNAMIC_COPY = GL_DYNAMIC_COPY
	};

public:
	explicit Buffer(Target target);

	/**
	 * Binds this buffer to the target. See GL::State::BindBuffer()
	 */
	void Bind() const;

	Target GetTarget() const;

	/**
	 * Resets the contents of the buffer.
	 * Use the usage pramater to specify usage hits to OpenGL. Default is
	 * STATIC_DRAW. See
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 * for more information on this.
	 */
	void SetDataEmpty(Usage usage = Usage::STATIC_DRAW);

	/**
	 * Set the contents of the buffer. Count elements will be read from the data
	 * pointer. Use the usage parameter to specify usage hits to OpenGL. Default
	 * is STATIC_DRAW. See
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 * for more information on this.
	 */
	template<typename T>
	void SetData(const T* data, size_t count, Usage usage = Usage::STATIC_DRAW) {
		Bind();
		glBufferData(GLenum(_target), count * sizeof(T), data, GLenum(usage));
	}

	/**
	 * Sets the contents of the buffer. All elements from the vector will be
	 * read. Use the usage parameter to specify usage hits to OpenGL.
	 * Default is STATIC_DRAW. See
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 * for more information on this.
	 */
	template<typename T>
	void SetData(const std::vector<T>& data, Usage usage = Usage::STATIC_DRAW) {
		SetData(data.data(), data.size(), usage);
	}

	/**
	 * Replaces part of the contents of the buffer. Count elements will be read
	 * from the data pointer. The first element will be put at the specified
	 * index, measured in elements (not bytes!).
	 */
	template<typename T>
	void SetSubData(size_t index, const T* data, size_t count) {
		Bind();
		glBufferSubData(GLenum(_target), index * sizeof(T), count * sizeof(T), data);
	}

	/**
	 * Replaces part of the contents of the buffer. All elements of the vector
	 * will be read. The first element will be put at the specified* index,
	 * measured in elements (not bytes!).
	 */
	template<typename T>
	void SetSubData(size_t index, const std::vector<T>& data) {
		SetSubData(index, data.data(), data.size());
	}

private:
	Target _target;

};

std::ostream& operator<<(std::ostream& out, Buffer::Target target);

}

#endif
