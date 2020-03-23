/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "_GL.h"

#if RE_GL_DEBUG
#include <iostream>
#endif

namespace rheel {

std::stack<_GL::_BindingState> _GL::_STATE_STACK = std::stack<_GL::_BindingState>({
	_BindingState()
});

GLuint _GL::_window_framebuffer_width;
GLuint _GL::_window_framebuffer_height;

bool _GL::_BindBuffer(GLuint handle, BufferTarget target) {
	if (_S().bound_buffer[int(target)] == handle) {
		return false;
	}

#if RE_GL_DEBUG
	Log::Info() << "glBindBuffer(" << int(target) << ", " << handle << ")" << std::endl;
#endif

	glBindBuffer(Target(target), handle);
	_S().bound_buffer[int(target)] = handle;

	return true;
}

bool _GL::_BindFramebuffer(GLuint handle, GLuint width, GLuint height, FramebufferTarget target) {
	if (_S().bound_framebuffer[int(target)] == handle) {
		return false;
	}

#if RE_GL_DEBUG
	Log::Info() << "glBindFramebuffer(" << int(target) << ", " << handle << ")" << std::endl;
#endif

	glBindFramebuffer(Target(target), handle);
	_S().bound_framebuffer[int(target)] = handle;
	_S().bound_framebuffer_sizes[int(target)] = { width, height };

	if (target == FramebufferTarget::DRAW) {

#if RE_GL_DEBUG
	Log::Info() << "glViewport(0, 0, " << width << ", " << height << ")" << std::endl;
#endif

		glViewport(0, 0, width, height);
	}

	return true;
}

bool _GL::_BindFramebuffer(GLuint handle, GLuint width, GLuint height) {
	return _BindFramebuffer(handle, width, height, FramebufferTarget::DRAW) ||
			_BindFramebuffer(handle, width, height, FramebufferTarget::READ);
}

bool _GL::_BindRenderbuffer(GLuint handle) {
	if (_S().bound_renderbuffer == handle) {
		return false;
	}

#if RE_GL_DEBUG
	Log::Info() << "glBindRenderbuffer(" << GL_RENDERBUFFER << ", " << handle << ")" << std::endl;
#endif

	glBindRenderbuffer(GL_RENDERBUFFER, handle);
	_S().bound_renderbuffer = handle;

	return true;
}

bool _GL::_BindTexture(GLuint handle, TextureTarget target, GLuint textureUnit) {

#if RE_GL_DEBUG
	Log::Info() << "glActiveTexture(GL_TEXTURE0 + " << textureUnit << ")" << std::endl;
#endif

	glActiveTexture(GL_TEXTURE0 + textureUnit);

	if (_S().bound_texture[int(target)][textureUnit] == handle) {
		return false;
	}

#if RE_GL_DEBUG
	Log::Info() << "glBindTexture(" << int(target) << ", " << handle << ")" << std::endl;
#endif

	glBindTexture(Target(target), handle);
	_S().bound_texture[int(target)][textureUnit] = handle;

	return true;
}

bool _GL::_BindVertexArray(GLuint handle) {
	if (_S().bound_vertex_array == handle) {
		return false;
	}

#if RE_GL_DEBUG
	Log::Info() << "glBindVertexArray(" << handle << ")" << std::endl;
#endif

	glBindVertexArray(handle);
	_S().bound_vertex_array = handle;

	return true;
}

bool _GL::BindBuffer(const GLhandle& handle, BufferTarget target) {
	return _GL::_BindBuffer((GLuint) handle, target);
}

bool _GL::BindFramebuffer(const GLhandle& handle, GLuint width, GLuint height, FramebufferTarget target) {
	return _BindFramebuffer((GLuint) handle, width, height, target);
}

bool _GL::BindFramebuffer(const GLhandle& handle, GLuint width, GLuint height) {
	return _BindFramebuffer((GLuint) handle, width, height);
}

bool _GL::BindRenderbuffer(const GLhandle& handle) {
	return _BindRenderbuffer((GLuint) handle);
}

bool _GL::BindTexture(const GLhandle& handle, TextureTarget target, GLuint textureUnit) {
	return _BindTexture((GLuint) handle, target, textureUnit);
}

bool _GL::BindVertexArray(const GLhandle& handle) {
	return _BindVertexArray((GLuint) handle);
}

bool _GL::ClearBufferBinding(BufferTarget target, bool force) {
	if (force) {
		return BindBuffer(GLhandle(), target);
	} else if (_S().bound_buffer[int(target)] != 0) {
		_S().bound_buffer[int(target)] = 0;
		return true;
	}

	return false;
}

bool _GL::ClearFramebufferBinding(FramebufferTarget target) {
	return BindFramebuffer(GLhandle(), _window_framebuffer_width,
			_window_framebuffer_height, target);
}

bool _GL::ClearFramebufferBinding() {
	return ClearFramebufferBinding(FramebufferTarget::DRAW) ||
			ClearFramebufferBinding(FramebufferTarget::READ);
}

bool _GL::ClearRenderbufferBinding() {
	return BindRenderbuffer(GLhandle());
}

bool _GL::ClearTextureBinding(TextureTarget target, GLuint textureUnit) {
	return BindTexture(GLhandle(), target, textureUnit);
}

bool _GL::ClearVertexArrayBinding() {
	return BindVertexArray(GLhandle());
}

void _GL::PushState() {
	_STATE_STACK.push(_BindingState(_S()));
}

void _GL::PopState() {
	if (_STATE_STACK.size() == 1) {
		throw std::runtime_error("Stack underflow");
	}

	// save the top state
	_BindingState popped = _STATE_STACK.top();

	// get the target state
	_STATE_STACK.pop();
	_BindingState target = _S();

	// re-add the top state
	_STATE_STACK.push(popped);

	// restore the state
	for (int i = 0; i <= int(_GL::BufferTarget::LAST); i++) {
		_BindBuffer(target.bound_buffer[i], _GL::BufferTarget(i));
	}

	for (int i = 0; i <= int(_GL::FramebufferTarget::LAST); i++) {
		_BindFramebuffer(target.bound_framebuffer[i], target.bound_framebuffer_sizes[i].x, target.bound_framebuffer_sizes[i].y, _GL::FramebufferTarget(i));
	}

	// TODO: actually do this (oops)

	// actually pop the state stack
	_STATE_STACK.pop();
}

void _GL::SetWindowFramebufferSize(GLuint width, GLuint height) {
	_window_framebuffer_width = width;
	_window_framebuffer_height = height;
}

}
