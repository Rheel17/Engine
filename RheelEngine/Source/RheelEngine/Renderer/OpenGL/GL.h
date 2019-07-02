#ifndef GL_UTILS_H_
#define GL_UTILS_H_
#include "../../_common.h"

#include <GL/glew.h>

#include <cassert>
#include <functional>
#include <map>
#include <memory>

namespace rheel {

class GL;

class RE_API GLhandle {
	friend class GL;

	typedef std::function<void(GLsizei, GLuint *)> _Generator;
	typedef std::function<void(GLsizei, GLuint *)> _Deleter;

public:
	// default constructor to create a invalid GLhandle
	GLhandle() : _valid(false), _handle(nullptr) {}

	/*
	 * When the handle is valid, it returns the OpenGL id of the object. If
	 * the handle is not valid, it always returns 0.
	 */
	inline operator GLuint() const { return _valid ? *_handle : 0; }

	/*
	 * Returns whether the handle is valid.
	 */
	inline operator bool() const { return _valid; }

private:
	GLhandle(_Generator generator, _Deleter deleter) :
			_valid(true) {

		// create the OpenGL object
		GLuint *id = new GLuint;
		generator(1, id);

		// create the handle, with a custom deleter when all instances
		// of this object go out of scope
		_handle = std::shared_ptr<GLuint>(id, [deleter](GLuint *id) {
			deleter(1, id);
		});
	}

	bool _valid;
	std::shared_ptr<GLuint> _handle;

};

class RE_API GL {

public:
	enum class BufferTarget {
			ARRAY, ATOMIC_COUNTER, COPY_READ, COPY_WRITE, DISPATCH_INDIRECT, ELEMENT_ARRAY, PIXEL_PACK,
			PIXEL_UNPACK, QUERY, SHADER_STORAGE, TEXTURE, TRANSFORM_FEEDBACK, UNIFORM
	};

	enum class FramebufferTarget {
			DRAW, READ
	};

	enum class TextureTarget {
			TEXTURE_1D, TEXTURE_2D, TEXTURE_3D, TEXTURE_1D_ARRAY, TEXTURE_2D_ARRAY, TEXTURE_RECTANGLE,
			TEXTURE_CUBE_MAP, TEXTURE_CUBE_MAP_ARRAY, TEXTURE_BUFFER, TEXTURE_2D_MULTISAMPLE,
			TEXTURE_2D_MULTISAMPLE_ARRAY
	};

public:
	enum class FilterFunction {
		NEAREST                = GL_NEAREST,
		LINEAR                 = GL_LINEAR,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR,
	};

	enum class WrapParameter {
		CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
		REPEAT          = GL_REPEAT,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT
	};

private:
	constexpr static GLenum _BUFFER_BINDING_TARGET_GL[13] = {
			GL_ARRAY_BUFFER, GL_ATOMIC_COUNTER_BUFFER, GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
			GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER,
			GL_QUERY_BUFFER, GL_SHADER_STORAGE_BUFFER, GL_TEXTURE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER,
			GL_UNIFORM_BUFFER
	};

	constexpr static GLenum _FRAMEBUFFER_TARGET_GL[2] = {
			GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER
	};

	constexpr static GLenum _TEXTURE_TARGET_GL[11] = {
			GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_RECTANGLE,
			GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BUFFER, GL_TEXTURE_2D_MULTISAMPLE,
			GL_TEXTURE_2D_MULTISAMPLE_ARRAY
	};

	// Delete all the default con/de-structors, copy/assignment, etc.
private:
	GL() = delete;
	~GL() = delete;
	GL(const GL&) = delete;
	GL operator=(const GL&) = delete;

public:
	// GLhandle creators

	inline static GLhandle GenBuffer()       { return GLhandle(glGenBuffers,       glDeleteBuffers      ); }
	inline static GLhandle GenFramebuffer()  { return GLhandle(glGenFramebuffers,  glDeleteFramebuffers ); }
	inline static GLhandle GenRenderbuffer() { return GLhandle(glGenRenderbuffers, glDeleteRenderbuffers); }
	inline static GLhandle GenTexture()      { return GLhandle(glGenTextures,      glDeleteTextures     ); }
	inline static GLhandle GenVertexArray()  { return GLhandle(glGenVertexArrays,  glDeleteVertexArrays ); }

	inline static GLhandle CreateProgram() {
		return GLhandle([](GLsizei count, GLuint *ids) {
			for (GLsizei i = 0; i < count; i++) {
				ids[i] = glCreateProgram();
			}
		}, [](GLsizei count, GLuint *ids) {
			for (GLsizei i = 0; i < count; i++) {
				glDeleteProgram(ids[i]);
			}
		});
	}

	// Binders

	static bool BindBuffer(GLhandle handle, BufferTarget target);
	static bool BindFramebuffer(GLhandle handle, FramebufferTarget target);
	static bool BindFramebuffer(GLhandle handle);
	static bool BindRenderbuffer(GLhandle handle);
	static bool BindTexture(GLhandle handle, TextureTarget target, GLuint textureUnit = 0);
	static bool BindVertexArray(GLhandle handle);

	static bool ClearBufferBinding(BufferTarget target, bool force = false);
	static bool ClearFramebufferBinding(FramebufferTarget target);
	static bool ClearFramebufferBinding();
	static bool ClearRenderbufferBinding();
	static bool ClearTextureBinding(TextureTarget target, GLuint textureUnit = 0);
	static bool ClearVertexArrayBinding();

	inline static GLenum Target(BufferTarget target) { return _BUFFER_BINDING_TARGET_GL[int(target)]; }
	inline static GLenum Target(FramebufferTarget target) { return _FRAMEBUFFER_TARGET_GL[int(target)]; }
	inline static GLenum Target(TextureTarget target) { return _TEXTURE_TARGET_GL[int(target)]; }

private:
	static GLuint _BOUND_BUFFER[13];
	static GLuint _BOUND_FRAMEBUFFER[2];
	static GLuint _BOUND_RENDERBUFFER;
	static GLuint _BOUND_TEXTURE[11][16];
	static GLuint _BOUND_VERTEX_ARRAY;

	// TODO: create state-version of this, so push/pop is possible. Probably put everything in
	// struct, so all the binding state is pushed/popped at once.

};

}

#endif
