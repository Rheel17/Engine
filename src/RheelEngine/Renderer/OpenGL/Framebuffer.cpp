/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Framebuffer.h"

#include <iomanip>

#include "State.h"

namespace rheel::GL {

std::unique_ptr<Framebuffer> Framebuffer::_default_framebuffer(nullptr);

Framebuffer::Framebuffer(unsigned viewportWidth, unsigned viewportHeight) :
		_viewport_width(viewportWidth), _viewport_height(viewportHeight) {}

Framebuffer::Framebuffer(const Framebuffer& original, unsigned newWidth, unsigned newHeight) :
		_viewport_width(newWidth), _viewport_height(newHeight) {

	for (const auto& [attachment, texture] : original._attached_textures) {
		_AttachTexture(texture.internalFormat, texture.format, attachment);
	}

	for (const auto& [attachment, texture] : original._attached_multisample_textures) {
		_AttachTextureMultisample(texture.internalFormat, texture.samples, attachment);
	}

	for (const auto& [attachment, buffer] : original._attached_renderbuffers) {
		_AttachRenderbuffer(buffer.internalFormat, attachment);
	}

	for (const auto& [attachment, buffer] : original._attached_multisample_renderbuffers) {
		_AttachRenderbufferMultisample(buffer.internalFormat, buffer.samples, attachment);
	}

	SetDrawBuffers(original._draw_buffers);
}

Framebuffer::Framebuffer(uvec2 defaultViewport) :
		Object(0), _viewport_width(defaultViewport.x), _viewport_height(defaultViewport.y) {}

void Framebuffer::BindForDrawing() const {
	State::BindFramebuffer(Target::DRAW, *this);
}

void Framebuffer::BindForReading() const {
	State::BindFramebuffer(Target::READ, *this);
}

void Framebuffer::Clear(BitField buffersToClear) const {
	BindForDrawing();
	glClear(GLenum(buffersToClear));
}

void Framebuffer::Blit(ivec4 inBounds, ivec4 outBounds, BitField buffers, bool linear) const {
	BindForReading();
	glBlitFramebuffer(
			inBounds.x, inBounds.y, inBounds.x + inBounds.z, inBounds.y + inBounds.w,
			outBounds.x, outBounds.y, outBounds.x + outBounds.z, outBounds.y + outBounds.w,
			GLenum(buffers),
			linear ? GL_LINEAR : GL_NEAREST);
}

unsigned Framebuffer::GetViewportWidth() const {
	return _viewport_width;
}

unsigned Framebuffer::GetViewportHeight() const {
	return _viewport_height;
}

void Framebuffer::AttachTexture(InternalFormat internalFormat, Format format, unsigned colorAttachment) {
	_AttachTexture(internalFormat, format, GL_COLOR_ATTACHMENT0 + colorAttachment);
}

void Framebuffer::AttachTexture(InternalFormat internalFormat, Format format, Attachment attachment) {
	_AttachTexture(internalFormat, format, GLenum(attachment));
}

void Framebuffer::AttachTextureMultisample(InternalFormat internalFormat, unsigned samples, unsigned colorAttachment) {
	_AttachTextureMultisample(internalFormat, samples, GL_COLOR_ATTACHMENT0 + colorAttachment);
}

void Framebuffer::AttachTextureMultisample(InternalFormat internalFormat, unsigned samples, Attachment attachment) {
	_AttachTextureMultisample(internalFormat, samples, GLenum(attachment));
}

void Framebuffer::AttachRenderbuffer(InternalFormat format, unsigned colorAttachment) {
	_AttachRenderbuffer(format, GL_COLOR_ATTACHMENT0 + colorAttachment);
}

void Framebuffer::AttachRenderbuffer(InternalFormat format, Attachment attachment) {
	_AttachRenderbuffer(format, GLenum(attachment));
}

void Framebuffer::AttachRenderbufferMultisample(InternalFormat internalFormat, unsigned samples, unsigned colorAttachment) {
	_AttachRenderbufferMultisample(internalFormat, samples, GL_COLOR_ATTACHMENT0 + colorAttachment);
}

void Framebuffer::AttachRenderbufferMultisample(InternalFormat internalFormat, unsigned samples, Attachment attachment) {
	_AttachRenderbufferMultisample(internalFormat, samples, GLenum(attachment));
}

Texture2D& Framebuffer::GetTextureAttachment(unsigned colorAttachment) {
	return _GetAttachment(_attached_textures, GL_COLOR_ATTACHMENT0 + colorAttachment).texture;
}

const Texture2D& Framebuffer::GetTextureAttachment(unsigned colorAttachment) const {
	return _GetAttachment(_attached_textures, GL_COLOR_ATTACHMENT0 + colorAttachment).texture;
}

Texture2D& Framebuffer::GetTextureAttachment(Attachment attachment) {
	return _GetAttachment(_attached_textures, GLenum(attachment)).texture;
}

const Texture2D& Framebuffer::GetTextureAttachment(Attachment attachment) const {
	return _GetAttachment(_attached_textures, GLenum(attachment)).texture;
}

Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(unsigned colorAttachment) {
	return _GetAttachment(_attached_multisample_textures, GL_COLOR_ATTACHMENT0 + colorAttachment).texture;
}

const Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(unsigned colorAttachment) const {
	return _GetAttachment(_attached_multisample_textures, GL_COLOR_ATTACHMENT0 + colorAttachment).texture;
}

Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(Attachment attachment) {
	return _GetAttachment(_attached_multisample_textures, GLenum(attachment)).texture;
}

const Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(Attachment attachment) const {
	return _GetAttachment(_attached_multisample_textures, GLenum(attachment)).texture;
}

Renderbuffer& Framebuffer::GetRenderbufferAttachment(unsigned colorAttachment) {
	return _GetAttachment(_attached_renderbuffers, GL_COLOR_ATTACHMENT0 + colorAttachment).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferAttachment(unsigned colorAttachment) const {
	return _GetAttachment(_attached_renderbuffers, GL_COLOR_ATTACHMENT0 + colorAttachment).buffer;
}

Renderbuffer& Framebuffer::GetRenderbufferAttachment(Attachment attachment) {
	return _GetAttachment(_attached_renderbuffers, GLenum(attachment)).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferAttachment(Attachment attachment) const {
	return _GetAttachment(_attached_renderbuffers, GLenum(attachment)).buffer;
}

Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(unsigned colorAttachment) {
	return _GetAttachment(_attached_multisample_renderbuffers, GL_COLOR_ATTACHMENT0 + colorAttachment).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(unsigned colorAttachment) const {
	return _GetAttachment(_attached_multisample_renderbuffers, GL_COLOR_ATTACHMENT0 + colorAttachment).buffer;
}

Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(Attachment attachment) {
	return _GetAttachment(_attached_multisample_renderbuffers, GLenum(attachment)).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(Attachment attachment) const {
	return _GetAttachment(_attached_multisample_renderbuffers, GLenum(attachment)).buffer;
}

Framebuffer::AttachmentType Framebuffer::GetAttachmentType(unsigned colorAttachment) const {
	if (_attached_textures.find(GL_COLOR_ATTACHMENT0 + colorAttachment) != _attached_textures.end()) {
		return AttachmentType::TEXTURE;
	}

	if (_attached_multisample_textures.find(GL_COLOR_ATTACHMENT0 + colorAttachment) != _attached_multisample_textures.end()) {
		return AttachmentType::TEXTURE_MULTISAMPLE;
	}

	if (_attached_renderbuffers.find(GL_COLOR_ATTACHMENT0 + colorAttachment) != _attached_renderbuffers.end()) {
		return AttachmentType::RENDERBUFFER;
	}

	if (_attached_multisample_renderbuffers.find(GL_COLOR_ATTACHMENT0 + colorAttachment) != _attached_multisample_renderbuffers.end()) {
		return AttachmentType::RENDERBUFFER_MULTISAMPLE;
	}

	return AttachmentType::NONE;
}

Framebuffer::AttachmentType Framebuffer::GetAttachmentType(Framebuffer::Attachment attachment) const {
	if (_attached_textures.find(GLenum(attachment)) != _attached_textures.end()) {
		return AttachmentType::TEXTURE;
	}

	if (_attached_multisample_textures.find(GLenum(attachment)) != _attached_multisample_textures.end()) {
		return AttachmentType::TEXTURE_MULTISAMPLE;
	}

	if (_attached_renderbuffers.find(GLenum(attachment)) != _attached_renderbuffers.end()) {
		return AttachmentType::RENDERBUFFER;
	}

	if (_attached_multisample_renderbuffers.find(GLenum(attachment)) != _attached_multisample_renderbuffers.end()) {
		return AttachmentType::RENDERBUFFER_MULTISAMPLE;
	}

	return AttachmentType::NONE;
}

void Framebuffer::SetDrawBuffers(std::vector<unsigned> colorAttachments) {
	BindForDrawing();

	std::vector<GLenum> data;
	for (unsigned a : colorAttachments) {
		data.push_back(GL_COLOR_ATTACHMENT0 + a);
	}

	glDrawBuffers(data.size(), data.data());
	_CheckStatus();

	_draw_buffers = std::move(colorAttachments);
}

void Framebuffer::InitializeDefaultFramebuffer(uvec2 screenSize) {
	_default_framebuffer = std::unique_ptr<Framebuffer>(new Framebuffer(screenSize));
}

const Framebuffer& Framebuffer::DefaultFramebuffer() {
	return *_default_framebuffer;
}

uvec2 Framebuffer::DefaultViewport() {
	return { _default_framebuffer->_viewport_width, _default_framebuffer->_viewport_height };
}

void Framebuffer::_AttachTexture(InternalFormat internalFormat, Format format, GLenum attachment) {
	if (_HasAttachment(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the texture in place
	Texture2D& texture = (_attached_textures[attachment] = { Texture2D(), internalFormat, format }).texture;

	// initialize texture
	texture.SetEmpty(internalFormat, _viewport_width, _viewport_height, format);
	texture.SetWrapParameterS(Texture::WrapParameter::CLAMP_TO_EDGE);
	texture.SetWrapParameterT(Texture::WrapParameter::CLAMP_TO_EDGE);
	texture.SetMinifyingFilter(Texture::FilterFunction::LINEAR);
	texture.SetMagnificationFilter(Texture::FilterFunction::LINEAR);

	// attach the texture to the framebuffer
	BindForDrawing();
	glFramebufferTexture2D(GLenum(Target::DRAW), attachment, GLenum(Texture::Target::TEXTURE_2D), texture.GetName(), 0);
}

void Framebuffer::_AttachTextureMultisample(InternalFormat internalFormat, unsigned samples, GLenum attachment) {
	if (_HasAttachment(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the texture in place
	Texture2DMultisample& texture = (_attached_multisample_textures[attachment] = { Texture2DMultisample(), internalFormat, samples }).texture;

	// initialize texture
	texture.Initialize(internalFormat, _viewport_width, _viewport_height, samples);

	// attach the texture to the framebuffer
	BindForDrawing();
	glFramebufferTexture2D(GLenum(Target::DRAW), attachment, GLenum(Texture::Target::TEXTURE_2D_MULTISAMPLE), texture.GetName(), 0);
}

void Framebuffer::_AttachRenderbuffer(InternalFormat internalFormat, GLenum attachment) {
	if (_HasAttachment(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the renderbuffer in place
	Renderbuffer& buffer = (_attached_renderbuffers[attachment] = { Renderbuffer(), internalFormat }).buffer;

	// initialize buffer
	buffer.SetStorage(internalFormat, _viewport_width, _viewport_height);

	// attach the renderbuffer to the framebuffer
	BindForDrawing();
	glFramebufferRenderbuffer(GLenum(Target::DRAW), attachment, GL_RENDERBUFFER, buffer.GetName());
}

void Framebuffer::_AttachRenderbufferMultisample(InternalFormat internalFormat, unsigned samples, GLenum attachment) {
	if (_HasAttachment(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the renderbuffer in place
	Renderbuffer& buffer = (_attached_multisample_renderbuffers[attachment] = { Renderbuffer(), internalFormat, samples }).buffer;

	// initialize buffer
	buffer.SetStorageMultisample(internalFormat, _viewport_width, _viewport_height, samples);

	// attach the renderbuffer to the framebuffer
	BindForDrawing();
	glFramebufferRenderbuffer(GLenum(Target::DRAW), attachment, GL_RENDERBUFFER, buffer.GetName());
}

void Framebuffer::_CheckStatus() const {
	GLenum status = glCheckFramebufferStatus(GLenum(Target::DRAW));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Log::Error() << "Framebuffer is not complete (0x" << std::hex << status << ")" << std::endl;
		abort();
	}
}

bool Framebuffer::_HasAttachment(GLenum attachment) const {
	return !(_attached_textures.find(attachment) == _attached_textures.end() &&
			 _attached_multisample_textures.find(attachment) == _attached_multisample_textures.end() &&
			 _attached_renderbuffers.find(attachment) == _attached_renderbuffers.end());
}

}