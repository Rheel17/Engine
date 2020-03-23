/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef GLBUFFER_H_
#define GLBUFFER_H_
#include "../../_common.h"

#include "_GL.h"

namespace rheel {

class RE_API _GLBuffer {

public:
	enum Usage {
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
	/**
	 * Creates an _OpenGL buffer, with the specified target.
	 */
	_GLBuffer(_GL::BufferTarget target);

	/**
	 * Returns the internal _OpenGL ID of this buffer.
	 */
	GLuint ID() const;

	/**
	 * Returns the binding target of this buffer.
	 */
	_GL::BufferTarget Target() const;

	/**
	 * Binds this buffer to the previously specified target.
	 */
	void Bind() const;

	/**
	 * Clears the binding for the specified target.
	 *
	 * Note that calling this method gives no guarantee that the buffer is not bound
	 * after the call.
	 *
	 * Use the force parameter to actually force a glBindBuffer(target, 0) call. If
	 * force is false (the default), calling this method will only guarantee that the
	 * next Bind() call will call glBindBuffer.
	 */
	static void ClearBinding(_GL::BufferTarget target, bool force = false);

	/**
	 * Set the contents of the buffer. Size bytes will be read from the data pointer.
	 * Use the usage parameter to specify usage hits to _OpenGL. Default is STATIC_DRAW.
	 * See https://www.khronos.org/registry/_OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 * for more information on this.
	 */
	void SetData(const void *data, GLsizeiptr size, Usage usage = STATIC_DRAW);

	/**
	 * Sets the contents of the buffer. The complete vector will be read in its order.
	 * Use the usage parameter to specify usage hits to _OpenGL. Default is STATIC_DRAW.
	 * See https://www.khronos.org/registry/_OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 * for more information on this.
	 */
	template<typename T, typename _Alloc>
	void SetData(const std::vector<T, _Alloc>& data, Usage usage = STATIC_DRAW) {
		SetData(data.data(), data.size() * sizeof(T), usage);
	}

private:
	GLhandle _id;
	_GL::BufferTarget _target;

};

}

#endif
