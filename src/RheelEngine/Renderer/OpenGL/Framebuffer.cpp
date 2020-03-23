/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Framebuffer.h"

#include <iomanip>

#include "State.h"

namespace rheel::GL {

Framebuffer::Framebuffer(unsigned viewportWidth, unsigned viewportHeight) :
		_viewport_width(viewportWidth), _viewport_height(viewportHeight) {}

void Framebuffer::BindForDrawing() const {
	State::BindFramebuffer(Target::DRAW, *this);
}

void Framebuffer::BindForReading() const {
	State::BindFramebuffer(Target::READ, *this);
}

unsigned Framebuffer::GetViewportWidth() const {
	return _viewport_width;
}

unsigned Framebuffer::GetViewportHeight() const {
	return _viewport_height;
}

void Framebuffer::AttachTexture(InternalFormat format, unsigned colorAttachment) {
	AttachTexture(Texture2D(), colorAttachment);
	auto& texture = GetTextureAttachment(colorAttachment);

	texture.SetEmpty(format, _viewport_width, _viewport_height);
	texture.SetWrapParameterS(Texture::WrapParameter::CLAMP_TO_EDGE);
	texture.SetWrapParameterT(Texture::WrapParameter::CLAMP_TO_EDGE);
	texture.SetMinifyingFilter(Texture::FilterFunction::LINEAR);
	texture.SetMagnificationFilter(Texture::FilterFunction::LINEAR);
}

void Framebuffer::AttachTexture(InternalFormat format, Attachment attachment) {
	AttachTexture(Texture2D(), attachment);
	auto& texture = GetTextureAttachment(attachment);

	texture.SetEmpty(format, _viewport_width, _viewport_height);
	texture.SetWrapParameterS(Texture::WrapParameter::CLAMP_TO_EDGE);
	texture.SetWrapParameterT(Texture::WrapParameter::CLAMP_TO_EDGE);
	texture.SetMinifyingFilter(Texture::FilterFunction::LINEAR);
	texture.SetMagnificationFilter(Texture::FilterFunction::LINEAR);
}

void Framebuffer::AttachTexture(Texture2D texture, unsigned colorAttachment) {
	if (_attached_textures.find(GL_COLOR_ATTACHMENT0 + colorAttachment) == _attached_textures.end() ||
			_attached_renderbuffers.find(GL_COLOR_ATTACHMENT0 + colorAttachment) == _attached_renderbuffers.end()) {
		Log::Error() << "Color attachment" << colorAttachment << "already has an attachment" << std::endl;
		abort();
	}

	GLuint name = texture.GetName();
	_attached_textures[GL_COLOR_ATTACHMENT0 + colorAttachment] = std::move(texture);

	BindForDrawing();
	glFramebufferTexture2D(GLenum(Target::DRAW), GL_COLOR_ATTACHMENT0 + colorAttachment, GL_TEXTURE_2D, name, 0);
}

void Framebuffer::AttachTexture(Texture2D texture, Attachment attachment) {
	if (_attached_textures.find(GLenum(attachment)) == _attached_textures.end() ||
		_attached_renderbuffers.find(GLenum(attachment)) == _attached_renderbuffers.end()) {
		Log::Error() << "Attachment 0x" << std::hex << GLenum(attachment) << "already has an attachment" << std::endl;
		abort();
	}

	GLuint name = texture.GetName();
	_attached_textures[GLenum(attachment)] = std::move(texture);

	BindForDrawing();
	glFramebufferTexture2D(GLenum(Target::DRAW), GLenum(attachment), GL_TEXTURE_2D, name, 0);
}

void Framebuffer::AttachRenderbuffer(InternalFormat format, unsigned colorAttachment) {
	AttachRenderbuffer(Renderbuffer(), colorAttachment);
	GetRenderbufferAttachment(colorAttachment).SetStorage(format, _viewport_width, _viewport_height);
}

void Framebuffer::AttachRenderbuffer(InternalFormat format, Attachment attachment) {
	AttachRenderbuffer(Renderbuffer(), attachment);
	GetRenderbufferAttachment(attachment).SetStorage(format, _viewport_width, _viewport_height);
}

void Framebuffer::AttachRenderbuffer(Renderbuffer renderbuffer, unsigned colorAttachment) {
	if (_attached_textures.find(GL_COLOR_ATTACHMENT0 + colorAttachment) == _attached_textures.end() ||
		_attached_renderbuffers.find(GL_COLOR_ATTACHMENT0 + colorAttachment) == _attached_renderbuffers.end()) {
		Log::Error() << "Color attachment" << colorAttachment << "already has an attachment" << std::endl;
		abort();
	}

	GLuint name = renderbuffer.GetName();
	_attached_renderbuffers[GL_COLOR_ATTACHMENT0 + colorAttachment] = std::move(renderbuffer);

	BindForDrawing();
	glFramebufferRenderbuffer(GLenum(Target::DRAW), GL_COLOR_ATTACHMENT0 + colorAttachment, GL_RENDERBUFFER, name);
}

void Framebuffer::AttachRenderbuffer(Renderbuffer renderbuffer, Attachment attachment) {
	if (_attached_textures.find(GLenum(attachment)) == _attached_textures.end() ||
		_attached_renderbuffers.find(GLenum(attachment)) == _attached_renderbuffers.end()) {
		Log::Error() << "Attachment 0x" << std::hex << GLenum(attachment) << "already has an attachment" << std::endl;
		abort();
	}

	GLuint name = renderbuffer.GetName();
	_attached_renderbuffers[GLenum(attachment)] = std::move(renderbuffer);

	BindForDrawing();
	glFramebufferRenderbuffer(GLenum(Target::DRAW), GLenum(attachment), GL_RENDERBUFFER, name);
}

Texture2D& Framebuffer::GetTextureAttachment(unsigned colorAttachment) {
	auto iter = _attached_textures.find(GL_COLOR_ATTACHMENT0 + colorAttachment);
	if (iter == _attached_textures.end()) {
		Log::Error() << "No texture attached to color attachment " << colorAttachment << std::endl;
		abort();
	}
	return iter->second;
}

const Texture2D& Framebuffer::GetTextureAttachment(unsigned colorAttachment) const {
	auto iter = _attached_textures.find(GL_COLOR_ATTACHMENT0 + colorAttachment);
	if (iter == _attached_textures.end()) {
		Log::Error() << "No texture attached to color attachment " << colorAttachment << std::endl;
		abort();
	}
	return iter->second;
}

Texture2D& Framebuffer::GetTextureAttachment(Attachment attachment) {
	auto iter = _attached_textures.find(GLenum(attachment));
	if (iter == _attached_textures.end()) {
		Log::Error() << "No texture attached to attachment 0x" << std::hex << GLenum(attachment) << std::endl;
		abort();
	}
	return iter->second;
}

const Texture2D& Framebuffer::GetTextureAttachment(Attachment attachment) const {
	auto iter = _attached_textures.find(GLenum(attachment));
	if (iter == _attached_textures.end()) {
		Log::Error() << "No texture attached to attachment 0x" << std::hex << GLenum(attachment) << std::endl;
		abort();
	}
	return iter->second;
}

Renderbuffer& Framebuffer::GetRenderbufferAttachment(unsigned colorAttachment) {
	auto iter = _attached_renderbuffers.find(GL_COLOR_ATTACHMENT0 + colorAttachment);
	if (iter == _attached_renderbuffers.end()) {
		Log::Error() << "No renderbuffer attached to color attachment " << colorAttachment << std::endl;
		abort();
	}
	return iter->second;
}

const Renderbuffer& Framebuffer::GetRenderbufferAttachment(unsigned colorAttachment) const {
	auto iter = _attached_renderbuffers.find(GL_COLOR_ATTACHMENT0 + colorAttachment);
	if (iter == _attached_renderbuffers.end()) {
		Log::Error() << "No renderbuffer attached to color attachment " << colorAttachment << std::endl;
		abort();
	}
	return iter->second;
}

Renderbuffer& Framebuffer::GetRenderbufferAttachment(Attachment attachment) {
	auto iter = _attached_renderbuffers.find(GLenum(attachment));
	if (iter == _attached_renderbuffers.end()) {
		Log::Error() << "No renderbuffer attached to attachment 0x" << std::hex << GLenum(attachment) << std::endl;
		abort();
	}
	return iter->second;
}

const Renderbuffer& Framebuffer::GetRenderbufferAttachment(Attachment attachment) const {
	auto iter = _attached_renderbuffers.find(GLenum(attachment));
	if (iter == _attached_renderbuffers.end()) {
		Log::Error() << "No renderbuffer attached to attachment 0x" << std::hex << GLenum(attachment) << std::endl;
		abort();
	}
	return iter->second;
}

void Framebuffer::SetDrawBuffers(std::vector<unsigned> colorAttachments) {
	BindForDrawing();
	glDrawBuffers(colorAttachments.size(), colorAttachments.data());
	_CheckStatus();
}

void Framebuffer::_CheckStatus() const {
	GLenum status = glCheckFramebufferStatus(GLenum(Target::DRAW));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Log::Error() << "Framebuffer is not complete (0x" << std::hex << status << ")" << std::endl;
		abort();
	}
}

}