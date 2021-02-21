/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_FRAMEBUFFER_H
#define RHEELENGINE_FRAMEBUFFER_H
#include "../../_common.h"

#include "Texture2D.h"
#include "Texture2DMultisample.h"
#include "Renderbuffer.h"

namespace rheel::gl {

OPENGL_GEN_FUNCTION(glGenFramebuffers, gen_framebuffers_);
OPENGL_DELETE_FUNCTION(glDeleteFramebuffers, delete_framebuffers_);

class RE_API Framebuffer : public Object<gen_framebuffers_, delete_framebuffers_> {
	friend class Window;

public:
	enum class Target {
		DRAW = GL_DRAW_FRAMEBUFFER,
		READ = GL_READ_FRAMEBUFFER,
		BOTH = GL_FRAMEBUFFER
	};

	enum class Attachment {
		DEPTH = GL_DEPTH_ATTACHMENT,
		STENCIL = GL_STENCIL_ATTACHMENT,
		DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
	};

	enum class BitField {
		COLOR = GL_COLOR_BUFFER_BIT,
		DEPTH = GL_DEPTH_BUFFER_BIT,
		STENCIL = GL_STENCIL_BUFFER_BIT,

		COLOR_DEPTH = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
		COLOR_STENCIL = GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
		DEPTH_STENCIL = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,

		COLOR_DEPTH_STENCIL = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
	};

	enum class AttachmentType {
		NONE, TEXTURE, TEXTURE_MULTISAMPLE, RENDERBUFFER, RENDERBUFFER_MULTISAMPLE
	};

private:
	struct texture_attachment {
		Texture2D texture;
		InternalFormat internal_format;
		Format format;
	};

	struct texture_multisample_attachment {
		Texture2DMultisample texture;
		InternalFormat internal_format;
		unsigned samples;
	};

	struct renderbuffer_attachment {
		Renderbuffer buffer;
		InternalFormat internal_format;
	};

	struct renderbuffer_multisample_attachment {
		Renderbuffer buffer;
		InternalFormat internal_format;
		unsigned samples;
	};

public:
	Framebuffer(unsigned viewport_width, unsigned viewport_height);
	Framebuffer(const Framebuffer& original, unsigned new_width, unsigned new_height);

	void BindForDrawing() const;
	void BindForReading() const;

	void Clear(BitField buffers_to_clear) const;

	/**
	 * Blits the contents of this framebuffer to the currently draw-bound framebuffer.
	 * bounds format: { x, y, width, height }
	 */
	void Blit(ivec4 in_bounds, ivec4 out_bounds, BitField buffers, bool linear = false) const;

	unsigned GetViewportWidth() const;
	unsigned GetViewportHeight() const;

	/**
	 * Creates a texture attachment and attaches it to the specified color attachment.
	 * Retrieve the texture using GetTextureAttachment(colorAttachment).
	 */
	void AttachTexture(InternalFormat internal_format, Format format, unsigned color_attachment);

	/**
	 * Creates a texture attachment and attaches it to the specified attachment.
	 * Retrieve the texture using GetTextureAttachment(attachment).
	 */
	void AttachTexture(InternalFormat internal_format, Format format, Attachment attachment);

	/**
	 * Creates a texture attachment and attaches it to the specified color attachment.
	 * Retrieve the texture using GetTextureMultisampleAttachment(colorAttachment).
	 */
	void AttachTextureMultisample(InternalFormat internal_format, unsigned samples, unsigned color_attachment);

	/**
	 * Creates a texture attachment and attaches it to the specified attachment.
	 * Retrieve the texture using GetTextureMultisampleAttachment(attachment).
	 */
	void AttachTextureMultisample(InternalFormat internal_format, unsigned samples, Attachment attachment);

	/**
	 * Creates a renderbuffer and attaches it to the specified color attachment.
	 * Retrieve the buffer using GetRenderbuffer(colorAttachment).
	 */
	void AttachRenderbuffer(InternalFormat internal_format, unsigned color_attachment);

	/**
	 * Creates a renderbuffer and attaches it to the specified attachment.
	 * Retrieve the buffer using GetRenderbuffer(attachment).
	 */
	void AttachRenderbuffer(InternalFormat internal_format, Attachment attachment);

	/**
	 * Creates a renderbuffer attachment and attaches it to the specified attachment.
	 * Retrieve the buffer using GetRenderbufferMultisampleAttachment(colorAttachment).
	 */
	void AttachRenderbufferMultisample(InternalFormat internal_format, unsigned samples, unsigned color_attachment);

	/**
	 * Creates a renderbuffer attachment and attaches it to the specified attachment.
	 * Retrieve the buffer using GetRenderbufferMultisampleAttachment(attachment).
	 */
	void AttachRenderbufferMultisample(InternalFormat internal_format, unsigned samples, Attachment attachment);

	/**
	 * Returns a reference to the texture attachment at the specified color
	 * attachment
	 */
	Texture2D& GetTextureAttachment(unsigned color_attachment);

	/**
	 * Returns a reference to the texture attachment at the specified color
	 * attachment
	 */
	const Texture2D& GetTextureAttachment(unsigned color_attachment) const;

	/**
	 * Returns a reference to the texture attachment at the specified attachment
	 */
	Texture2D& GetTextureAttachment(Attachment attachment);

	/**
	 * Returns a reference to the texture attachment at the specified attachment
	 */
	const Texture2D& GetTextureAttachment(Attachment attachment) const;

	/**
	 * Returns a reference to the texture attachment at the specified color
	 * attachment
	 */
	Texture2DMultisample& GetTextureMultisampleAttachment(unsigned color_attachment);

	/**
	 * Returns a reference to the texture attachment at the specified color
	 * attachment
	 */
	const Texture2DMultisample& GetTextureMultisampleAttachment(unsigned color_attachment) const;

	/**
	 * Returns a reference to the texture attachment at the specified attachment
	 */
	Texture2DMultisample& GetTextureMultisampleAttachment(Attachment attachment);

	/**
	 * Returns a reference to the texture attachment at the specified attachment
	 */
	const Texture2DMultisample& GetTextureMultisampleAttachment(Attachment attachment) const;

	/**
	 * Returns a reference to the renderbuffer attachment at the specified
	 * color attachment
	 */
	Renderbuffer& GetRenderbufferAttachment(unsigned color_attachment);

	/**
	 * Returns a reference to the renderbuffer attachment at the specified
	 * color attachment
	 */
	const Renderbuffer& GetRenderbufferAttachment(unsigned color_attachment) const;

	/**
	 * Returns a reference to the renderbuffer attachment at the specified
	 * attachment
	 */
	Renderbuffer& GetRenderbufferAttachment(Attachment attachment);

	/**
	 * Returns a reference to the renderbuffer attachment at the specified
	 * attachment
	 */
	const Renderbuffer& GetRenderbufferAttachment(Attachment attachment) const;

	/**
	 * Returns a reference to the renderbuffer attachment at the specified
	 * color attachment
	 */
	Renderbuffer& GetRenderbufferMultisampleAttachment(unsigned color_attachment);

	/**
	 * Returns a reference to the renderbuffer attachment at the specified
	 * color attachment
	 */
	const Renderbuffer& GetRenderbufferMultisampleAttachment(unsigned color_attachment) const;

	/**
	 * Returns a reference to the renderbuffer attachment at the specified
	 * attachment
	 */
	Renderbuffer& GetRenderbufferMultisampleAttachment(Attachment attachment);

	/**
	 * Returns a reference to the renderbuffer attachment at the specified
	 * attachment
	 */
	const Renderbuffer& GetRenderbufferMultisampleAttachment(Attachment attachment) const;

	/**
	 * Returns the type of attachment attached to the specified color
	 * attachment
	 */
	AttachmentType GetAttachmentType(unsigned color_attachment) const;

	/**
	 * Returns the type of attachment attached to the specified attachment.
	 */
	AttachmentType GetAttachmentType(Attachment attachment) const;

	/**
	 * Set which color attachments can be drawn to. These color attachments
	 * must have either a texture or a renderbuffer attached to them.
	 */
	void SetDrawBuffers(std::vector<unsigned> color_attachments);

private:
	// constructor for the default framebuffer
	Framebuffer();

	void _attach_texture(InternalFormat internal_format, Format format, GLenum attachment);
	void _attach_texture_multisample(InternalFormat internal_format, unsigned samples, GLenum attachment);
	void _attach_renderbuffer(InternalFormat internal_format, GLenum attachment);
	void _attach_renderbuffer_multisample(InternalFormat internal_format, unsigned samples, GLenum attachment);

	template<typename T>
	const T& _get_attachment(const std::unordered_map<GLenum, T>& attachments, GLenum attachment) const {
		auto iter = attachments.find(attachment);
		if (iter == attachments.end()) {
			Log::Error() << "Nothing attached to attachment or getting the wrong type" << std::endl;
			abort();
		}

		return iter->second;
	}

	template<typename T>
	T& _get_attachment(std::unordered_map<GLenum, T>& attachments, GLenum attachment) {
		auto iter = attachments.find(attachment);
		if (iter == attachments.end()) {
			Log::Error() << "Nothing attached to attachment or getting the wrong type" << std::endl;
			abort();
		}

		return iter->second;
	}

	void _check_status() const;
	bool _has_attachment(GLenum attachment) const;

	unsigned _viewport_width;
	unsigned _viewport_height;
	std::vector<unsigned> _draw_buffers;

	std::unordered_map<GLenum, texture_attachment> _attached_textures;
	std::unordered_map<GLenum, texture_multisample_attachment> _attached_multisample_textures;
	std::unordered_map<GLenum, renderbuffer_attachment> _attached_renderbuffers;
	std::unordered_map<GLenum, renderbuffer_multisample_attachment> _attached_multisample_renderbuffers;

public:
	static Framebuffer DefaultFramebuffer();
	static uvec2 DefaultViewport();

};

std::ostream& operator<<(std::ostream& out, Framebuffer::Target target);

}

#endif
