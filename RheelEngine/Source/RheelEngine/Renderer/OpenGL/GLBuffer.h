#ifndef GLBUFFER_H_
#define GLBUFFER_H_
#include "../../_common.h"

#include "GL.h"

#include <vector>

namespace rheel {

class RE_API GLBuffer {

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
	 * Creates an OpenGL buffer, with the specified target.
	 */
	GLBuffer(GL::BufferTarget target);

	/**
	 * Returns the internal OpenGL ID of this buffer.
	 */
	GLuint GetID() const;

	/**
	 * Returns the binding target of this buffer.
	 */
	GL::BufferTarget Target() const;

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
	static void ClearBinding(GL::BufferTarget target, bool force = false);

	/**
	 * Set the contents of the buffer. Size bytes will be read from the data pointer.
	 * Use the usage parameter to specify usage hits to OpenGL. Default is STATIC_DRAW.
	 * See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 * for more information on this.
	 */
	void SetData(const void *data, GLsizeiptr size, Usage usage = STATIC_DRAW);

	/**
	 * Sets the contents of the buffer. The complete vector will be read in its order.
	 * Use the usage parameter to specify usage hits to OpenGL. Default is STATIC_DRAW.
	 * See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
	 * for more information on this.
	 */
	template<typename T>
	void SetData(const std::vector<T>& data, Usage usage = STATIC_DRAW) {
		SetData(data.data(), data.size() * sizeof(T), usage);
	}

private:
	GLhandle _id;
	GL::BufferTarget _target;

};

}

#endif
