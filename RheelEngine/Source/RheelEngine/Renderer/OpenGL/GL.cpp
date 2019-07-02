#include "GL.h"

#include <iostream>

namespace rheel {

GLuint GL::_BOUND_BUFFER[13] = { 0 };
GLuint GL::_BOUND_FRAMEBUFFER[2] = { 0 };
GLuint GL::_BOUND_RENDERBUFFER = { 0 };
GLuint GL::_BOUND_TEXTURE[11][16] = { 0 };
GLuint GL::_BOUND_VERTEX_ARRAY = { 0 };

bool GL::BindBuffer(GLhandle handle, BufferTarget target) {
	if (_BOUND_BUFFER[int(target)] == (GLuint) handle) {
		return false;
	}

	glBindBuffer(Target(target), handle);
	_BOUND_BUFFER[int(target)] = handle;

	return true;
}

bool GL::BindFramebuffer(GLhandle handle, FramebufferTarget target) {
	if (_BOUND_FRAMEBUFFER[int(target)] == (GLuint) handle) {
		return false;
	}

	glBindFramebuffer(Target(target), handle);
	_BOUND_FRAMEBUFFER[int(target)] = handle;

	return true;
}

bool GL::BindFramebuffer(GLhandle handle) {
	return BindFramebuffer(handle, FramebufferTarget::DRAW) ||
			BindFramebuffer(handle, FramebufferTarget::READ);
}

bool GL::BindRenderbuffer(GLhandle handle) {
	if (_BOUND_RENDERBUFFER == (GLuint) handle) {
		return false;
	}

	glBindRenderbuffer(GL_RENDERBUFFER, handle);
	_BOUND_RENDERBUFFER = handle;

	return true;
}

bool GL::BindTexture(GLhandle handle, TextureTarget target, GLuint textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);

	if (_BOUND_TEXTURE[int(target)][textureUnit] == (GLuint) handle) {
		return false;
	}

	glBindTexture(Target(target), (GLuint) handle);
	_BOUND_TEXTURE[int(target)][textureUnit] = handle;

	return true;
}

bool GL::BindVertexArray(GLhandle handle) {
	if (_BOUND_VERTEX_ARRAY == (GLuint) handle) {
		return false;
	}

	glBindVertexArray(handle);
	_BOUND_VERTEX_ARRAY = handle;

	return true;
}

bool GL::ClearBufferBinding(BufferTarget target, bool force) {
	if (force) {
		return BindBuffer(GLhandle(), target);
	} else if (_BOUND_BUFFER[int(target)] != 0) {
		_BOUND_BUFFER[int(target)] = 0;
		return true;
	}

	return false;
}

bool GL::ClearFramebufferBinding(FramebufferTarget target) {
	return BindFramebuffer(GLhandle(), target);
}

bool GL::ClearFramebufferBinding() {
	return ClearFramebufferBinding(FramebufferTarget::DRAW) ||
			ClearFramebufferBinding(FramebufferTarget::READ);
}

bool GL::ClearRenderbufferBinding() {
	return BindRenderbuffer(GLhandle());
}

bool GL::ClearTextureBinding(TextureTarget target, GLuint textureUnit) {
	return BindTexture(GLhandle(), target, textureUnit);
}

bool GL::ClearVertexArrayBinding() {
	return BindVertexArray(GLhandle());
}

}
