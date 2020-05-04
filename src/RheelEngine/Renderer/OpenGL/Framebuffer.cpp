/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Framebuffer.h"

#include <iomanip>

#include "Context.h"

namespace rheel::gl {

Framebuffer::Framebuffer(unsigned viewportWidth, unsigned viewportHeight) :
		_viewport_width(viewportWidth),
		_viewport_height(viewportHeight) {}

Framebuffer::Framebuffer(const Framebuffer& original, unsigned newWidth, unsigned newHeight) :
		_viewport_width(newWidth),
		_viewport_height(newHeight) {

	for (const auto&[attachment, texture] : original._attached_textures) {
		AttachTexture_(texture.internalFormat, texture.format, attachment);
	}

	for (const auto&[attachment, texture] : original._attached_multisample_textures) {
		AttachTextureMultisample_(texture.internal_format, texture.samples, attachment);
	}

	for (const auto&[attachment, buffer] : original._attached_renderbuffers) {
		AttachRenderbuffer_(buffer.internal_format, attachment);
	}

	for (const auto&[attachment, buffer] : original._attached_multisample_renderbuffers) {
		AttachRenderbufferMultisample_(buffer.internal_format, buffer.samples, attachment);
	}

	SetDrawBuffers(original._draw_buffers);
}

Framebuffer::Framebuffer() :
		Object(0) {

	auto viewport = DefaultViewport();
	_viewport_width = viewport.x;
	_viewport_height = viewport.y;
}

void Framebuffer::BindForDrawing() const {
	Context::Current().BindFramebuffer(Target::DRAW, *this);
}

void Framebuffer::BindForReading() const {
	Context::Current().BindFramebuffer(Target::READ, *this);
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
	AttachTexture_(internalFormat, format, GL_COLOR_ATTACHMENT0 + colorAttachment);
}

void Framebuffer::AttachTexture(InternalFormat internalFormat, Format format, Attachment attachment) {
	AttachTexture_(internalFormat, format, GLenum(attachment));
}

void Framebuffer::AttachTextureMultisample(InternalFormat internalFormat, unsigned samples, unsigned colorAttachment) {
	AttachTextureMultisample_(internalFormat, samples, GL_COLOR_ATTACHMENT0 + colorAttachment);
}

void Framebuffer::AttachTextureMultisample(InternalFormat internalFormat, unsigned samples, Attachment attachment) {
	AttachTextureMultisample_(internalFormat, samples, GLenum(attachment));
}

void Framebuffer::AttachRenderbuffer(InternalFormat format, unsigned colorAttachment) {
	AttachRenderbuffer_(format, GL_COLOR_ATTACHMENT0 + colorAttachment);
}

void Framebuffer::AttachRenderbuffer(InternalFormat format, Attachment attachment) {
	AttachRenderbuffer_(format, GLenum(attachment));
}

void Framebuffer::AttachRenderbufferMultisample(InternalFormat internalFormat, unsigned samples, unsigned colorAttachment) {
	AttachRenderbufferMultisample_(internalFormat, samples, GL_COLOR_ATTACHMENT0 + colorAttachment);
}

void Framebuffer::AttachRenderbufferMultisample(InternalFormat internalFormat, unsigned samples, Attachment attachment) {
	AttachRenderbufferMultisample_(internalFormat, samples, GLenum(attachment));
}

Texture2D& Framebuffer::GetTextureAttachment(unsigned colorAttachment) {
	return GetAttachment_(_attached_textures, GL_COLOR_ATTACHMENT0 + colorAttachment).texture;
}

const Texture2D& Framebuffer::GetTextureAttachment(unsigned colorAttachment) const {
	return GetAttachment_(_attached_textures, GL_COLOR_ATTACHMENT0 + colorAttachment).texture;
}

Texture2D& Framebuffer::GetTextureAttachment(Attachment attachment) {
	return GetAttachment_(_attached_textures, GLenum(attachment)).texture;
}

const Texture2D& Framebuffer::GetTextureAttachment(Attachment attachment) const {
	return GetAttachment_(_attached_textures, GLenum(attachment)).texture;
}

Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(unsigned colorAttachment) {
	return GetAttachment_(_attached_multisample_textures, GL_COLOR_ATTACHMENT0 + colorAttachment).texture;
}

const Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(unsigned colorAttachment) const {
	return GetAttachment_(_attached_multisample_textures, GL_COLOR_ATTACHMENT0 + colorAttachment).texture;
}

Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(Attachment attachment) {
	return GetAttachment_(_attached_multisample_textures, GLenum(attachment)).texture;
}

const Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(Attachment attachment) const {
	return GetAttachment_(_attached_multisample_textures, GLenum(attachment)).texture;
}

Renderbuffer& Framebuffer::GetRenderbufferAttachment(unsigned colorAttachment) {
	return GetAttachment_(_attached_renderbuffers, GL_COLOR_ATTACHMENT0 + colorAttachment).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferAttachment(unsigned colorAttachment) const {
	return GetAttachment_(_attached_renderbuffers, GL_COLOR_ATTACHMENT0 + colorAttachment).buffer;
}

Renderbuffer& Framebuffer::GetRenderbufferAttachment(Attachment attachment) {
	return GetAttachment_(_attached_renderbuffers, GLenum(attachment)).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferAttachment(Attachment attachment) const {
	return GetAttachment_(_attached_renderbuffers, GLenum(attachment)).buffer;
}

Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(unsigned colorAttachment) {
	return GetAttachment_(_attached_multisample_renderbuffers, GL_COLOR_ATTACHMENT0 + colorAttachment).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(unsigned colorAttachment) const {
	return GetAttachment_(_attached_multisample_renderbuffers, GL_COLOR_ATTACHMENT0 + colorAttachment).buffer;
}

Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(Attachment attachment) {
	return GetAttachment_(_attached_multisample_renderbuffers, GLenum(attachment)).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(Attachment attachment) const {
	return GetAttachment_(_attached_multisample_renderbuffers, GLenum(attachment)).buffer;
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
	CheckStatus_();

	_draw_buffers = std::move(colorAttachments);
}

Framebuffer Framebuffer::DefaultFramebuffer() {
	return Framebuffer();
}

uvec2 Framebuffer::DefaultViewport() {
	return Context::Current().GetDefaultViewport();
}

void Framebuffer::AttachTexture_(InternalFormat internalFormat, Format format, GLenum attachment) {
	if (HasAttachment_(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the texture in place
	Texture2D& texture = (_attached_textures[attachment] = { Texture2D(), internalFormat, format }).texture;

	// initialize texture
	{
		gl::ContextScope cs;

		texture.SetEmpty(internalFormat, _viewport_width, _viewport_height, format);
		texture.SetWrapParameterS(Texture::WrapParameter::CLAMP_TO_EDGE);
		texture.SetWrapParameterT(Texture::WrapParameter::CLAMP_TO_EDGE);
		texture.SetMinifyingFilter(Texture::FilterFunction::LINEAR);
		texture.SetMagnificationFilter(Texture::FilterFunction::LINEAR);
	}

	// attach the texture to the framebuffer
	BindForDrawing();
	glFramebufferTexture2D(GLenum(Target::DRAW), attachment, GLenum(Texture::Target::TEXTURE_2D), texture.GetName(), 0);
}

void Framebuffer::AttachTextureMultisample_(InternalFormat internalFormat, unsigned samples, GLenum attachment) {
	if (HasAttachment_(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the texture in place
	Texture2DMultisample& texture = (_attached_multisample_textures[attachment] = { Texture2DMultisample(), internalFormat, samples }).texture;

	// initialize texture
	{
		gl::ContextScope cs;
		texture.Initialize(internalFormat, _viewport_width, _viewport_height, samples);
	}

	// attach the texture to the framebuffer
	BindForDrawing();
	glFramebufferTexture2D(GLenum(Target::DRAW), attachment, GLenum(Texture::Target::TEXTURE_2D_MULTISAMPLE), texture.GetName(), 0);
}

void Framebuffer::AttachRenderbuffer_(InternalFormat internalFormat, GLenum attachment) {
	if (HasAttachment_(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the renderbuffer in place
	Renderbuffer& buffer = (_attached_renderbuffers[attachment] = { Renderbuffer(), internalFormat }).buffer;

	// initialize buffer
	{
		gl::ContextScope cs;
		buffer.SetStorage(internalFormat, _viewport_width, _viewport_height);
	}

	// attach the renderbuffer to the framebuffer
	BindForDrawing();
	glFramebufferRenderbuffer(GLenum(Target::DRAW), attachment, GL_RENDERBUFFER, buffer.GetName());
}

void Framebuffer::AttachRenderbufferMultisample_(InternalFormat internalFormat, unsigned samples, GLenum attachment) {
	if (HasAttachment_(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the renderbuffer in place
	Renderbuffer& buffer = (_attached_multisample_renderbuffers[attachment] = { Renderbuffer(), internalFormat, samples }).buffer;

	// initialize buffer
	{
		gl::ContextScope cs;
		buffer.SetStorageMultisample(internalFormat, _viewport_width, _viewport_height, samples);
	}

	// attach the renderbuffer to the framebuffer
	BindForDrawing();
	glFramebufferRenderbuffer(GLenum(Target::DRAW), attachment, GL_RENDERBUFFER, buffer.GetName());
}

void Framebuffer::CheckStatus_() const {
	GLenum status = glCheckFramebufferStatus(GLenum(Target::DRAW));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Log::Error() << "Framebuffer is not complete (0x" << std::hex << status << ")" << std::endl;
		abort();
	}
}

bool Framebuffer::HasAttachment_(GLenum attachment) const {
	return !(_attached_textures.find(attachment) == _attached_textures.end() &&
			_attached_multisample_textures.find(attachment) == _attached_multisample_textures.end() &&
			_attached_renderbuffers.find(attachment) == _attached_renderbuffers.end());
}

std::ostream& operator<<(std::ostream& out, Framebuffer::Target target) {
	switch (target) {
		case Framebuffer::Target::DRAW:
			return out << "DRAW";
		case Framebuffer::Target::READ:
			return out << "READ";
		case Framebuffer::Target::BOTH:
			return out << "BOTH";
	}

	return out;
}

}