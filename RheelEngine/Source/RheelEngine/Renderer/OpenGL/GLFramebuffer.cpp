#include "GLFramebuffer.h"

#include <stdexcept>
#include <exception>
#include <iostream>

namespace rheel {

GLuint GLFramebuffer::_window_framebuffer_width;
GLuint GLFramebuffer::_window_framebuffer_height;

GLFramebuffer::GLFramebuffer(GLuint width, GLuint height, GLuint samples) :
		_width(width), _height(height), _samples(1), _created(false) {

	if (_width <= 0) {
		_width = 1;
	}

	if (_height <= 0) {
		_height = 1;
	}

	_id = GL::GenFramebuffer();
}

GLuint GLFramebuffer::GetID() const {
	return _id;
}

GLFramebuffer GLFramebuffer::ResizedCopy(GLuint width, GLuint height) {
	GLFramebuffer newBuffer(width, height, _samples);

	for (auto t : _texture_add_info) {
		newBuffer.AddTexture(std::get<0>(t), std::get<1>(t), std::get<2>(t));
	}

	for (auto r : _renderbuffer_add_info) {
		newBuffer.AddRenderbuffer(std::get<0>(r), std::get<1>(r));
	}

	if (_created) {
		newBuffer.Create();
	}

	return newBuffer;
}

void GLFramebuffer::Bind() const {
	if (GL::BindFramebuffer(_id)) {
		glViewport(0, 0, _width, _height);
	}
}

void GLFramebuffer::BindForReading() const {
	GL::BindFramebuffer(_id, GL::FramebufferTarget::READ);
}

void GLFramebuffer::BindForDrawing() const {
	if (GL::BindFramebuffer(_id, GL::FramebufferTarget::DRAW)) {
		glViewport(0, 0, _width, _height);
	}
}

void GLFramebuffer::ClearBinding() {
	if (GL::ClearFramebufferBinding()) {
		glViewport(0, 0, _window_framebuffer_width, _window_framebuffer_height);
	}
}

void GLFramebuffer::ClearBindingForReading() {
	GL::ClearFramebufferBinding(GL::FramebufferTarget::READ);
}

void GLFramebuffer::ClearBindingForDrawing() {
	if (GL::ClearFramebufferBinding(GL::FramebufferTarget::DRAW)) {
		glViewport(0, 0, _window_framebuffer_width, _window_framebuffer_height);
	}
}

void GLFramebuffer::AddTexture(GLint internalFormat, GLenum format, GLenum type) {
	if (_created) {
		throw std::runtime_error("Framebuffer already created");
	}

	Bind();

	// generate and bind the texture
	GLTexture2D texture(_width, _height);
	texture.Bind();

	// set texture parameters
	texture.SetWrapParameterS(GL::WrapParameter::CLAMP_TO_EDGE);
	texture.SetWrapParameterT(GL::WrapParameter::CLAMP_TO_EDGE);
	texture.SetMinifyingFilter(GL::FilterFunction::LINEAR);
	texture.SetMagnificationFilter(GL::FilterFunction::LINEAR);

	// upload empty texture data
	texture.SetData(internalFormat, format, type, nullptr);

	// add the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _textures.size(), GL_TEXTURE_2D, texture.GetID(), 0);

	_textures.push_back(texture);
	_texture_add_info.push_back({ internalFormat, format, type });
}

void GLFramebuffer::AddRenderbuffer(GLenum internalFormat, GLenum attachment) {
	if (_created) {
		throw std::runtime_error("Framebuffer already created");
	}

	Bind();

	// generate and bind the renderbuffer
	GLRenderbuffer renderbuffer(_width, _height, internalFormat);
	renderbuffer.Bind();

	// add the renderbuffer to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer.GetID());

	_renderbuffers.push_back(renderbuffer);
	_renderbuffer_add_info.push_back({ internalFormat, attachment });
}

void GLFramebuffer::Create() {
	if (_created) {
		return;
	}

	// collect the attachments
	std::vector<GLuint> attachments;
	for (unsigned int i = 0; i < _textures.size(); i++) {
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}

	// bind the framebuffer
	Bind();

	// add the attachments
	glDrawBuffers(attachments.size(), attachments.data());

	// check if the framebuffer is finished
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Framebuffer is not complete");
	}

	_created = true;
}

const std::vector<GLTexture2D>& GLFramebuffer::Textures() const {
	return _textures;
}

const std::vector<GLRenderbuffer>& GLFramebuffer::Renderbuffers() const {
	return _renderbuffers;
}

void GLFramebuffer::SetWindowFramebufferSize(GLuint width, GLuint height) {
	_window_framebuffer_width = width;
	_window_framebuffer_height = height;
}

}
