/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Framebuffer.h"

#include <iomanip>

#include "Context.h"

namespace rheel::gl {

Framebuffer::Framebuffer(unsigned viewport_width, unsigned viewport_height) :
		_viewport_width(viewport_width),
		_viewport_height(viewport_height) {}

Framebuffer::Framebuffer(const Framebuffer& original, unsigned new_width, unsigned new_height) :
		_viewport_width(new_width),
		_viewport_height(new_height) {

	for (const auto&[attachment, texture] : original._attached_textures) {
		_attach_texture(texture.internal_format, texture.format, attachment);
	}

	for (const auto&[attachment, texture] : original._attached_multisample_textures) {
		_attach_texture_multisample(texture.internal_format, texture.samples, attachment);
	}

	for (const auto&[attachment, buffer] : original._attached_renderbuffers) {
		_attach_renderbuffer(buffer.internal_format, attachment);
	}

	for (const auto&[attachment, buffer] : original._attached_multisample_renderbuffers) {
		_attach_renderbuffer_multisample(buffer.internal_format, buffer.samples, attachment);
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

void Framebuffer::Clear(BitField buffers_to_clear) const {
	BindForDrawing();
	glClear(GLenum(buffers_to_clear));
}

void Framebuffer::Blit(ivec4 in_bounds, ivec4 out_bounds, BitField buffers, bool linear) const {
	BindForReading();
	glBlitFramebuffer(
			in_bounds.x, in_bounds.y, in_bounds.x + in_bounds.z, in_bounds.y + in_bounds.w,
			out_bounds.x, out_bounds.y, out_bounds.x + out_bounds.z, out_bounds.y + out_bounds.w,
			GLenum(buffers),
			linear ? GL_LINEAR : GL_NEAREST);
}

unsigned Framebuffer::GetViewportWidth() const {
	return _viewport_width;
}

unsigned Framebuffer::GetViewportHeight() const {
	return _viewport_height;
}

void Framebuffer::AttachTexture(InternalFormat internal_format, Format format, unsigned color_attachment) {
	_attach_texture(internal_format, format, GL_COLOR_ATTACHMENT0 + color_attachment);
}

void Framebuffer::AttachTexture(InternalFormat internal_format, Format format, Attachment attachment) {
	_attach_texture(internal_format, format, GLenum(attachment));
}

void Framebuffer::AttachTextureMultisample(InternalFormat internal_format, unsigned samples, unsigned color_attachment) {
	_attach_texture_multisample(internal_format, samples, GL_COLOR_ATTACHMENT0 + color_attachment);
}

void Framebuffer::AttachTextureMultisample(InternalFormat internal_format, unsigned samples, Attachment attachment) {
	_attach_texture_multisample(internal_format, samples, GLenum(attachment));
}

void Framebuffer::AttachRenderbuffer(InternalFormat internal_format, unsigned color_attachment) {
	_attach_renderbuffer(internal_format, GL_COLOR_ATTACHMENT0 + color_attachment);
}

void Framebuffer::AttachRenderbuffer(InternalFormat internal_format, Attachment attachment) {
	_attach_renderbuffer(internal_format, GLenum(attachment));
}

void Framebuffer::AttachRenderbufferMultisample(InternalFormat internal_format, unsigned samples, unsigned color_attachment) {
	_attach_renderbuffer_multisample(internal_format, samples, GL_COLOR_ATTACHMENT0 + color_attachment);
}

void Framebuffer::AttachRenderbufferMultisample(InternalFormat internal_format, unsigned samples, Attachment attachment) {
	_attach_renderbuffer_multisample(internal_format, samples, GLenum(attachment));
}

Texture2D& Framebuffer::GetTextureAttachment(unsigned color_attachment) {
	return _get_attachment(_attached_textures, GL_COLOR_ATTACHMENT0 + color_attachment).texture;
}

const Texture2D& Framebuffer::GetTextureAttachment(unsigned color_attachment) const {
	return _get_attachment(_attached_textures, GL_COLOR_ATTACHMENT0 + color_attachment).texture;
}

Texture2D& Framebuffer::GetTextureAttachment(Attachment attachment) {
	return _get_attachment(_attached_textures, GLenum(attachment)).texture;
}

const Texture2D& Framebuffer::GetTextureAttachment(Attachment attachment) const {
	return _get_attachment(_attached_textures, GLenum(attachment)).texture;
}

Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(unsigned color_attachment) {
	return _get_attachment(_attached_multisample_textures, GL_COLOR_ATTACHMENT0 + color_attachment).texture;
}

const Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(unsigned color_attachment) const {
	return _get_attachment(_attached_multisample_textures, GL_COLOR_ATTACHMENT0 + color_attachment).texture;
}

Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(Attachment attachment) {
	return _get_attachment(_attached_multisample_textures, GLenum(attachment)).texture;
}

const Texture2DMultisample& Framebuffer::GetTextureMultisampleAttachment(Attachment attachment) const {
	return _get_attachment(_attached_multisample_textures, GLenum(attachment)).texture;
}

Renderbuffer& Framebuffer::GetRenderbufferAttachment(unsigned color_attachment) {
	return _get_attachment(_attached_renderbuffers, GL_COLOR_ATTACHMENT0 + color_attachment).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferAttachment(unsigned color_attachment) const {
	return _get_attachment(_attached_renderbuffers, GL_COLOR_ATTACHMENT0 + color_attachment).buffer;
}

Renderbuffer& Framebuffer::GetRenderbufferAttachment(Attachment attachment) {
	return _get_attachment(_attached_renderbuffers, GLenum(attachment)).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferAttachment(Attachment attachment) const {
	return _get_attachment(_attached_renderbuffers, GLenum(attachment)).buffer;
}

Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(unsigned color_attachment) {
	return _get_attachment(_attached_multisample_renderbuffers, GL_COLOR_ATTACHMENT0 + color_attachment).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(unsigned color_attachment) const {
	return _get_attachment(_attached_multisample_renderbuffers, GL_COLOR_ATTACHMENT0 + color_attachment).buffer;
}

Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(Attachment attachment) {
	return _get_attachment(_attached_multisample_renderbuffers, GLenum(attachment)).buffer;
}

const Renderbuffer& Framebuffer::GetRenderbufferMultisampleAttachment(Attachment attachment) const {
	return _get_attachment(_attached_multisample_renderbuffers, GLenum(attachment)).buffer;
}

Framebuffer::AttachmentType Framebuffer::GetAttachmentType(unsigned color_attachment) const {
	if (_attached_textures.find(GL_COLOR_ATTACHMENT0 + color_attachment) != _attached_textures.end()) {
		return AttachmentType::TEXTURE;
	}

	if (_attached_multisample_textures.find(GL_COLOR_ATTACHMENT0 + color_attachment) != _attached_multisample_textures.end()) {
		return AttachmentType::TEXTURE_MULTISAMPLE;
	}

	if (_attached_renderbuffers.find(GL_COLOR_ATTACHMENT0 + color_attachment) != _attached_renderbuffers.end()) {
		return AttachmentType::RENDERBUFFER;
	}

	if (_attached_multisample_renderbuffers.find(GL_COLOR_ATTACHMENT0 + color_attachment) != _attached_multisample_renderbuffers.end()) {
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

void Framebuffer::SetDrawBuffers(std::vector<unsigned> color_attachments) {
	BindForDrawing();

	std::vector<GLenum> data;
	for (unsigned a : color_attachments) {
		data.push_back(GL_COLOR_ATTACHMENT0 + a);
	}

	glDrawBuffers(data.size(), data.data());
	_check_status();

	_draw_buffers = std::move(color_attachments);
}

Framebuffer Framebuffer::DefaultFramebuffer() {
	return Framebuffer();
}

uvec2 Framebuffer::DefaultViewport() {
	return Context::Current().GetDefaultViewport();
}

void Framebuffer::_attach_texture(InternalFormat internal_format, Format format, GLenum attachment) {
	if (_has_attachment(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the texture in place
	Texture2D& texture = (_attached_textures[attachment] = { Texture2D(), internal_format, format }).texture;

	// initialize texture
	{
		gl::ContextScope cs;

		texture.SetEmpty(internal_format, _viewport_width, _viewport_height, format);
		texture.SetWrapParameterS(Texture::WrapParameter::CLAMP_TO_EDGE);
		texture.SetWrapParameterT(Texture::WrapParameter::CLAMP_TO_EDGE);
		texture.SetMinifyingFilter(Texture::FilterFunction::LINEAR);
		texture.SetMagnificationFilter(Texture::FilterFunction::LINEAR);
	}

	// attach the texture to the framebuffer
	BindForDrawing();
	glFramebufferTexture2D(GLenum(Target::DRAW), attachment, GLenum(Texture::Target::TEXTURE_2D), texture.GetName(), 0);
}

void Framebuffer::_attach_texture_multisample(InternalFormat internal_format, unsigned samples, GLenum attachment) {
	if (_has_attachment(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the texture in place
	Texture2DMultisample& texture = (_attached_multisample_textures[attachment] = { Texture2DMultisample(), internal_format, samples }).texture;

	// initialize texture
	{
		gl::ContextScope cs;
		texture.Initialize(internal_format, _viewport_width, _viewport_height, samples);
	}

	// attach the texture to the framebuffer
	BindForDrawing();
	glFramebufferTexture2D(GLenum(Target::DRAW), attachment, GLenum(Texture::Target::TEXTURE_2D_MULTISAMPLE), texture.GetName(), 0);
}

void Framebuffer::_attach_renderbuffer(InternalFormat internal_format, GLenum attachment) {
	if (_has_attachment(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the renderbuffer in place
	Renderbuffer& buffer = (_attached_renderbuffers[attachment] = { Renderbuffer(), internal_format }).buffer;

	// initialize buffer
	{
		gl::ContextScope cs;
		buffer.SetStorage(internal_format, _viewport_width, _viewport_height);
	}

	// attach the renderbuffer to the framebuffer
	BindForDrawing();
	glFramebufferRenderbuffer(GLenum(Target::DRAW), attachment, GL_RENDERBUFFER, buffer.GetName());
}

void Framebuffer::_attach_renderbuffer_multisample(InternalFormat internal_format, unsigned samples, GLenum attachment) {
	if (_has_attachment(attachment)) {
		Log::Error() << "Framebuffer already has attachment attached for 0x" << std::hex << attachment << std::endl;
		abort();
	}

	// default-construct the renderbuffer in place
	Renderbuffer& buffer = (_attached_multisample_renderbuffers[attachment] = { Renderbuffer(), internal_format, samples }).buffer;

	// initialize buffer
	{
		gl::ContextScope cs;
		buffer.SetStorageMultisample(internal_format, _viewport_width, _viewport_height, samples);
	}

	// attach the renderbuffer to the framebuffer
	BindForDrawing();
	glFramebufferRenderbuffer(GLenum(Target::DRAW), attachment, GL_RENDERBUFFER, buffer.GetName());
}

void Framebuffer::_check_status() const {
	GLenum status = glCheckFramebufferStatus(GLenum(Target::DRAW));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Log::Error() << "Framebuffer is not complete (0x" << std::hex << status << ")" << std::endl;
		abort();
	}
}

bool Framebuffer::_has_attachment(GLenum attachment) const {
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