/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_FRAMEBUFFER_H
#define RHEELENGINE_GL_FRAMEBUFFER_H
#include "../../_common.h"

#include "Texture2D.h"
#include "Renderbuffer.h"

namespace rheel::GL {

OPENGL_GENDEL_FUNCTION(glGenFramebuffers, _GenFramebuffers);
OPENGL_GENDEL_FUNCTION(glDeleteFramebuffers, _DeleteFramebuffers);

class RE_API Framebuffer : public Object<_GenFramebuffers, _DeleteFramebuffers> {

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

public:
	Framebuffer(unsigned viewportWidth, unsigned viewportHeight);

	void BindForDrawing() const;
	void BindForReading() const;

	unsigned GetViewportWidth() const;
	unsigned GetViewportHeight() const;

	/**
	 * Creates a texture attachment and attaches it to the specified color attachment.
	 */
	void AttachTexture(InternalFormat format, unsigned colorAttachment);

	/**
	 * Creates a texture attachment and attaches it to the specified attachment.
	 */
	void AttachTexture(InternalFormat format, Attachment attachment);

	/**
	 * Attaches the texture to the specified color attachment. Note that since textures
	 * cannot be copied, the texture needs to be either created in-place, or moved from
	 * another texture, which then will become invalid. To retrieve a reference to the
	 * created texture, use GetTextureAttachment() with the correct color attachment.
	 */
	void AttachTexture(Texture2D texture, unsigned colorAttachment);

	/**
	 * Attaches the texture to the specified attachment. Note that since textures cannot be
	 * copied, the texture needs to be either created in-place, or moved from another texture,
	 * which then will become invalid. To retrieve a reference to the created texture, use
	 * GetTextureAttachment() with the correct attachment.
	 */
	void AttachTexture(Texture2D texture, Attachment attachment);

	/**
	 * Creates a renderbuffer and attaches it to the specified color attachment.
	 */
	void AttachRenderbuffer(InternalFormat internalFormat, unsigned colorAttachment);

	/**
	 * Creates a renderbuffer and attaches it to the specified attachment.
	 */
	void AttachRenderbuffer(InternalFormat internalFormat, Attachment attachment);

	/**
	 * Attaches the texture to the specified color attachment. Note that since
	 * renderbuffers cannot be copied, the texture needs to be either created
	 * in-place, or moved from another renderbuffer, which then will become invalid.
	 */
	void AttachRenderbuffer(Renderbuffer renderbuffer, unsigned colorAttachment);

	/**
	 * Attaches the texture to the specified attachment. Note that since
	 * renderbuffers cannot be copied, the texture needs to be either created
	 * in-place, or moved from another renderbuffer, which then will become invalid.
	 */
	void AttachRenderbuffer(Renderbuffer renderbuffer, Attachment attachment);

	/**
	 * Returns a reference to the texture attachment at the specified color
	 * attachment
	 */
	Texture2D& GetTextureAttachment(unsigned colorAttachment);

	/**
	 * Returns a reference to the texture attachment at the specified color
	 * attachment
	 */
	const Texture2D& GetTextureAttachment(unsigned colorAttachment) const;


	/**
	 * Returns a reference to the texture attachment at the specified attachment
	 */
	Texture2D& GetTextureAttachment(Attachment attachment);

	/**
	 * Returns a reference to the texture attachment at the specified attachment
	 */
	const Texture2D& GetTextureAttachment(Attachment attachment) const;

	/**
	 * Returns a reference to the renderbuffer attachment at the specified
	 * color attachment
	 */
	Renderbuffer& GetRenderbufferAttachment(unsigned colorAttachment);

	/**
	 * Returns a reference to the renderbuffer attachment at the specified
	 * color attachment
	 */
	const Renderbuffer& GetRenderbufferAttachment(unsigned colorAttachment) const;

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
	 * Set which color attachments can be drawn to. These color attachments
	 * must have either a texture or a renderbuffer attached to them.
	 */
	void SetDrawBuffers(std::vector<unsigned> colorAttachments);

private:
	void _CheckStatus() const;

	unsigned _viewport_width;
	unsigned _viewport_height;

	std::unordered_map<GLenum, Texture2D> _attached_textures;
	std::unordered_map<GLenum, Renderbuffer> _attached_renderbuffers;

};

}

#endif
