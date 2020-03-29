/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_FRAMEBUFFER_H
#define RHEELENGINE_GL_FRAMEBUFFER_H
#include "../../_common.h"

#include "Texture2D.h"
#include "Texture2DMultisample.h"
#include "Renderbuffer.h"

namespace rheel::GL {

OPENGL_GEN_FUNCTION(glGenFramebuffers, _GenFramebuffers);
OPENGL_DELETE_FUNCTION(glDeleteFramebuffers, _DeleteFramebuffers);

class RE_API Framebuffer : public Object<_GenFramebuffers, _DeleteFramebuffers> {
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
	struct _TextureAttachment {
		Texture2D texture;
		InternalFormat internalFormat;
		Format format;
	};

	struct _TextureMultisampleAttachment {
		Texture2DMultisample texture;
		InternalFormat internalFormat;
		unsigned samples;
	};

	struct _RenderbufferAttachment {
		Renderbuffer buffer;
		InternalFormat internalFormat;
	};

	struct _RenderbufferMultisampleAttachment {
		Renderbuffer buffer;
		InternalFormat internalFormat;
		unsigned samples;
	};

public:
	Framebuffer(unsigned viewportWidth, unsigned viewportHeight);
	Framebuffer(const Framebuffer& original, unsigned newWidth, unsigned newHeight);

	void BindForDrawing() const;
	void BindForReading() const;

	void Clear(BitField buffersToClear) const;

	/**
	 * Blits the contents of this framebuffer to the currently draw-bound framebuffer.
	 * bounds format: { x, y, width, height }
	 */
	void Blit(ivec4 inBounds, ivec4 outBounds, BitField buffers, bool linear = false) const;

	unsigned GetViewportWidth() const;
	unsigned GetViewportHeight() const;

	/**
	 * Creates a texture attachment and attaches it to the specified color attachment.
	 * Retrieve the texture using GetTextureAttachment(colorAttachment).
	 */
	void AttachTexture(InternalFormat internalFormat, Format format, unsigned colorAttachment);

	/**
	 * Creates a texture attachment and attaches it to the specified attachment.
	 * Retrieve the texture using GetTextureAttachment(attachment).
	 */
	void AttachTexture(InternalFormat internalFormat, Format format, Attachment attachment);

	/**
	 * Creates a texture attachment and attaches it to the specified color attachment.
	 * Retrieve the texture using GetTextureMultisampleAttachment(colorAttachment).
	 */
	void AttachTextureMultisample(InternalFormat internalFormat, unsigned samples, unsigned colorAttachment);

	/**
	 * Creates a texture attachment and attaches it to the specified attachment.
	 * Retrieve the texture using GetTextureMultisampleAttachment(attachment).
	 */
	void AttachTextureMultisample(InternalFormat internalFormat, unsigned samples, Attachment attachment);

	/**
	 * Creates a renderbuffer and attaches it to the specified color attachment.
	 * Retrieve the buffer using GetRenderbuffer(colorAttachment).
	 */
	void AttachRenderbuffer(InternalFormat internalFormat, unsigned colorAttachment);

	/**
	 * Creates a renderbuffer and attaches it to the specified attachment.
	 * Retrieve the buffer using GetRenderbuffer(attachment).
	 */
	void AttachRenderbuffer(InternalFormat internalFormat, Attachment attachment);

	/**
	 * Creates a renderbuffer attachment and attaches it to the specified attachment.
	 * Retrieve the buffer using GetRenderbufferMultisampleAttachment(colorAttachment).
	 */
	void AttachRenderbufferMultisample(InternalFormat internalFormat, unsigned samples, unsigned colorAttachment);

	/**
	 * Creates a renderbuffer attachment and attaches it to the specified attachment.
	 * Retrieve the buffer using GetRenderbufferMultisampleAttachment(attachment).
	 */
	void AttachRenderbufferMultisample(InternalFormat internalFormat, unsigned samples, Attachment attachment);

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
	 * Returns a reference to the texture attachment at the specified color
	 * attachment
	 */
	Texture2DMultisample& GetTextureMultisampleAttachment(unsigned colorAttachment);

	/**
	 * Returns a reference to the texture attachment at the specified color
	 * attachment
	 */
	const Texture2DMultisample& GetTextureMultisampleAttachment(unsigned colorAttachment) const;

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
	 * Returns a reference to the renderbuffer attachment at the specified
	 * color attachment
	 */
	Renderbuffer& GetRenderbufferMultisampleAttachment(unsigned colorAttachment);

	/**
	 * Returns a reference to the renderbuffer attachment at the specified
	 * color attachment
	 */
	const Renderbuffer& GetRenderbufferMultisampleAttachment(unsigned colorAttachment) const;

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
	AttachmentType GetAttachmentType(unsigned colorAttachment) const;

	/**
	 * Returns the type of attachment attached to the specified attachment.
	 */
	AttachmentType GetAttachmentType(Attachment attachment) const;

	/**
	 * Set which color attachments can be drawn to. These color attachments
	 * must have either a texture or a renderbuffer attached to them.
	 */
	void SetDrawBuffers(std::vector<unsigned> colorAttachments);

private:
	// constructor for the default framebuffer
	Framebuffer(uvec2 defaultViewport);

	void _AttachTexture(InternalFormat internalFormat, Format format, GLenum attachment);
	void _AttachTextureMultisample(InternalFormat internalFormat, unsigned samples, GLenum attachment);
	void _AttachRenderbuffer(InternalFormat internalFormat, GLenum attachment);
	void _AttachRenderbufferMultisample(InternalFormat internalFormat, unsigned samples, GLenum attachment);

	template<typename T>
	const T& _GetAttachment(const std::unordered_map<GLenum, T>& attachments, GLenum attachment) const {
		auto iter = attachments.find(attachment);
		if (iter == attachments.end()) {
			Log::Error() << "Nothing attached to attachment or getting the wrong type" << std::endl;
			abort();
		}

		return iter->second;
	}

	template<typename T>
	T& _GetAttachment(std::unordered_map<GLenum, T>& attachments, GLenum attachment) {
		auto iter = attachments.find(attachment);
		if (iter == attachments.end()) {
			Log::Error() << "Nothing attached to attachment or getting the wrong type" << std::endl;
			abort();
		}

		return iter->second;
	}

	void _CheckStatus() const;
	bool _HasAttachment(GLenum attachment) const;

	unsigned _viewport_width;
	unsigned _viewport_height;
	std::vector<unsigned> _draw_buffers;

	std::unordered_map<GLenum, _TextureAttachment> _attached_textures;
	std::unordered_map<GLenum, _TextureMultisampleAttachment> _attached_multisample_textures;
	std::unordered_map<GLenum, _RenderbufferAttachment> _attached_renderbuffers;
	std::unordered_map<GLenum, _RenderbufferMultisampleAttachment> _attached_multisample_renderbuffers;

public:
	static void InitializeDefaultFramebuffer(uvec2 screenSize);
	static const Framebuffer& DefaultFramebuffer();
	static uvec2 DefaultViewport();

private:
	static std::unique_ptr<Framebuffer> _default_framebuffer;

};

}

#endif
