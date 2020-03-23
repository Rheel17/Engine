/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_BUFFER_H
#define RHEELENGINE_GL_BUFFER_H
#include "../../_common.h"

#include "Object.h"

OPENGL_GENDEL_FUNCTION(glGenBuffers, _GenBuffers)
OPENGL_GENDEL_FUNCTION(glDeleteBuffers, _DeleteBuffers)

namespace rheel::GL {

class RE_API Buffer : public Object<_GenBuffers, _DeleteBuffers> {

public:
	enum class Target {
		ARRAY, ATOMIC_COUNTER, COPY_READ, COPY_WRITE, DISPATCH_INDIRECT, ELEMENT_ARRAY,
		PIXEL_PACK, PIXEL_UNPACK, QUERY, SHADER_STORAGE, TEXTURE, TRANSFORM_FEEDBACK,
		UNIFORM
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
	 * Set the contents of the buffer. Size bytes will be read from the data pointer.
	 * Use the usage parameter to specify usage hits to OpenGL. Default is STATIC_DRAW.
	 * See https://www.khronos.org/registry/_OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 * for more information on this.
	 */
	void SetData(const void *data, ptrdiff_t size, Usage usage = Usage::STATIC_DRAW);

	/**
	 * Set the contents of the buffer. Count elements will be read from the data pointer.
	 * Use the usage parameter to specify usage hits to OpenGL. Default is STATIC_DRAW.
	 * See https://www.khronos.org/registry/_OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 * for more information on this.
	 */
	template<class T>
	void SetData(const T *data, size_t count, Usage usage = Usage::STATIC_DRAW) {
		SetData(data, count * sizeof(T), usage);
	}

private:
	Target _target;

};

}

#endif
