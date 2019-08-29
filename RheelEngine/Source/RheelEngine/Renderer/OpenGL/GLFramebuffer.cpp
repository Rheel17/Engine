#include "GLFramebuffer.h"

#include <stdexcept>
#include <exception>
#include <iostream>

namespace rheel {

GLFramebuffer::GLFramebuffer(GLuint width, GLuint height, GLuint samples, bool forceMultisampled) :
		_width(width), _height(height), _samples(samples),
		_is_multisampled(samples != 1 || forceMultisampled), _created(false) {

	if (_width <= 0) {
		_width = 1;
	}

	if (_height <= 0) {
		_height = 1;
	}

	_id = GL::GenFramebuffer();
}

GLuint GLFramebuffer::ID() const {
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

void GLFramebuffer::AddTexture(GLint internalFormat, GLenum format) {
	// get the maximum color attachment
	int maxColorAttachments;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

	// find an unused color attachment
	for (int i = 0; i < maxColorAttachments; i++) {
		if (_used_color_attachments.find(GL_COLOR_ATTACHMENT0 + i) == _used_color_attachments.end()) {
			AddTexture(internalFormat, format, GL_COLOR_ATTACHMENT0 + i);
			return;
		}
	}

	throw std::runtime_error("Ran out of color attachments (max=" + std::to_string(maxColorAttachments) + ")");
}

void GLFramebuffer::AddTexture(GLint internalFormat, GLenum format, GLenum attachment) {
	if (_created) {
		throw std::runtime_error("Framebuffer already created");
	}

	// check for a valid attachment
	bool color = _IsColorAttachment(attachment);
	if (color) {
		if (_used_color_attachments.find(attachment) != _used_color_attachments.end()) {
			throw std::runtime_error("Attachment " + std::to_string(attachment) + " already in use.");
		}
	} else {
		if (attachment != GL_DEPTH_ATTACHMENT && attachment != GL_STENCIL_ATTACHMENT) {
			throw std::runtime_error("Invalid attachment");
		}

		if (_used_other_attachments.find(attachment) != _used_other_attachments.end()) {
			throw std::runtime_error("Attachment " + std::to_string(attachment) + " already in use.");
		}
	}

	Bind();

	if (_is_multisampled) {
		// generate and bind the texture
		GLTexture2DMultisample texture(_width, _height, _samples);
		texture.Bind();

		// initialize texture
		texture.Initialize(internalFormat);

		// add the texture to the framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D_MULTISAMPLE, texture.ID(), 0);

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
		texture.InitializeEmpty(format);

		// add the texture to the framebuffer
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture.ID(), 0);

		_textures.push_back(texture);
	}

	_texture_add_info.push_back({ internalFormat, format, attachment });

	if (color) {
		_used_color_attachments.insert(attachment);
	} else {
		_used_other_attachments.insert(attachment);
	}
}

void GLFramebuffer::AddRenderbuffer(GLenum internalFormat, GLenum attachment) {
	if (_created) {
		throw std::runtime_error("Framebuffer already created");
	}

	// check for a valid attachment
	bool color = _IsColorAttachment(attachment);
	if (color) {
		if (_used_color_attachments.find(attachment) != _used_color_attachments.end()) {
			throw std::runtime_error("Attachment " + std::to_string(attachment) + " already in use.");
		}
	} else {
		if (attachment != GL_DEPTH_ATTACHMENT && attachment != GL_STENCIL_ATTACHMENT) {
			throw std::runtime_error("Invalid attachment");
		}

		if (_used_other_attachments.find(attachment) != _used_other_attachments.end()) {
			throw std::runtime_error("Attachment " + std::to_string(attachment) + " already in use.");
		}
	}

	Bind();

	// generate and bind the renderbuffer
	GLRenderbuffer renderbuffer(_width, _height, internalFormat, _samples, _is_multisampled);
	renderbuffer.Bind();

	// add the renderbuffer to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer.ID());

	_renderbuffers.push_back(renderbuffer);
	_renderbuffer_add_info.push_back({ internalFormat, attachment });

	if (color) {
		_used_color_attachments.insert(attachment);
	} else {
		_used_other_attachments.insert(attachment);
	}
}

void GLFramebuffer::Create() {
	if (_created) {
		return;
	}

	// bind the framebuffer
	Bind();

	// set the color buffers
	std::vector<GLenum> colorAttachments;
	for (GLenum colorAttachment : _used_color_attachments) {
		colorAttachments.push_back(colorAttachment);
	}

	glDrawBuffers(colorAttachments.size(), colorAttachments.data());

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

bool GLFramebuffer::_IsColorAttachment(GLenum attachment) {
	if (attachment < GL_COLOR_ATTACHMENT0) {
		return false;
	}

	int maxColorAttachments;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

	return attachment < (GLenum) (GL_COLOR_ATTACHMENT0 + maxColorAttachments);
}

}
