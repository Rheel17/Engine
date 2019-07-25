#include "GLFramebuffer.h"

#include <stdexcept>
#include <exception>
#include <iostream>

namespace rheel {

GLFramebuffer::GLFramebuffer(GLuint width, GLuint height, GLuint samples, bool forceMultisampled) :
		_width(width), _height(height), _samples(samples), _texture_count(0),
		_is_multisampled(samples != 1 || forceMultisampled), _created(false) {

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
	GL::BindFramebuffer(_id, _width, _height);
}

void GLFramebuffer::BindForReading() const {
	GL::BindFramebuffer(_id, _width, _height, GL::FramebufferTarget::READ);
}

void GLFramebuffer::BindForDrawing() const {
	GL::BindFramebuffer(_id, _width, _height, GL::FramebufferTarget::DRAW);
}

void GLFramebuffer::ClearBinding() {
	GL::ClearFramebufferBinding();
}

void GLFramebuffer::ClearBindingForReading() {
	GL::ClearFramebufferBinding(GL::FramebufferTarget::READ);
}

void GLFramebuffer::ClearBindingForDrawing() {
	GL::ClearFramebufferBinding(GL::FramebufferTarget::DRAW);
}

void GLFramebuffer::AddTexture(GLint internalFormat, GLenum format, GLenum type) {
	if (_created) {
		throw std::runtime_error("Framebuffer already created");
	}

	Bind();

	if (_is_multisampled) {
		// generate and bind the texture
		GLTexture2DMultisample texture(_width, _height, _samples);
		texture.Bind();

		// initialize texture
		texture.Initialize(internalFormat);

		// add the texture to the framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _texture_count, GL_TEXTURE_2D_MULTISAMPLE, texture.GetID(), 0);

		_multisample_textures.push_back(texture);
	} else {
		// generate and bind the texture
		GLTexture2D texture(_width, _height, internalFormat);
		texture.Bind();

		// set texture parameters
		texture.SetWrapParameterS(GL::WrapParameter::CLAMP_TO_EDGE);
		texture.SetWrapParameterT(GL::WrapParameter::CLAMP_TO_EDGE);
		texture.SetMinifyingFilter(GL::FilterFunction::LINEAR);
		texture.SetMagnificationFilter(GL::FilterFunction::LINEAR);

		// upload empty texture data
		texture.InitializeEmpty();

		// add the texture to the framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _texture_count, GL_TEXTURE_2D, texture.GetID(), 0);

		_textures.push_back(texture);
	}

	_texture_add_info.push_back({ internalFormat, format, type });
	_texture_count++;
}

void GLFramebuffer::AddRenderbuffer(GLenum internalFormat, GLenum attachment) {
	if (_created) {
		throw std::runtime_error("Framebuffer already created");
	}

	Bind();

	// generate and bind the renderbuffer
	GLRenderbuffer renderbuffer(_width, _height, internalFormat, _samples, _is_multisampled);
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
	for (unsigned int i = 0; i < _texture_count; i++) {
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}

	// bind the framebuffer
	Bind();

	// add the attachments
	glDrawBuffers(attachments.size(), attachments.data());

	// check if the framebuffer is finished
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::string error = "Framebuffer is not complete (" + std::to_string(status) + ")";
		throw std::runtime_error(error);
	}

	_created = true;
}

const std::vector<GLTexture2DMultisample>& GLFramebuffer::MultisampleTextures() const {
	return _multisample_textures;
}

const std::vector<GLTexture2D>& GLFramebuffer::Textures() const {
	return _textures;
}

const std::vector<GLRenderbuffer>& GLFramebuffer::Renderbuffers() const {
	return _renderbuffers;
}

}
