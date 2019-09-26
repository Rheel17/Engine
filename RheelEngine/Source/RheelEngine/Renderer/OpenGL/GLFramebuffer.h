/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef GLFRAMEBUFFER_H_
#define GLFRAMEBUFFER_H_
#include "../../_common.h"

#include <unordered_set>

#include "GLTexture2D.h"
#include "GLTexture2DMultisample.h"
#include "GLRenderbuffer.h"

namespace rheel {

class RE_API GLFramebuffer {

public:
	/**
	 * Creates a framebuffer of the specified dimensions. Note that both
	 * width and heigth must be at least 1. When any of them is 0, they are
	 * automatically set to 1. The samples variable is used in multisampling
	 * effects. Each attachment will use that amount of samples.
	 */
	GLFramebuffer(GLuint width, GLuint height, GLuint samples = 1,
			bool forceMultisampled = false);

	/**
	 * Returns the internal OpenGL ID of this texture.
	 */
	GLuint ID() const;

	/**
	 * Resizes the framebuffer to the specified dimensions. Note that both
	 * width and height must be at least 1. When any of them is 0, they are
	 * automatically set to 1.
	 */
	GLFramebuffer ResizedCopy(GLuint width, GLuint height);

	/**
	 * Binds this framebuffer to both the read and write buffer. Use
	 * GLFramebuffer::ClearBinding() to reset the framebuffer to the default
	 * one.
	 */
	void Bind() const;

	/**
	 * Binds this framebuffer to the read buffer. Use
	 * GLFramebuffer::ClearBindingForReading() to reset the read framebuffer
	 * to the default one.
	 */
	void BindForReading() const;

	/**
	 * Binds this framebuffer to the write buffer. Use
	 * GLFramebuffer::ClearBindingForWriting() to reset the write framebuffer
	 * to the derfault one.
	 */
	void BindForDrawing() const;

	/**
	 * Resets the read and write framebuffer to the default framebuffer (0).
	 */
	static void ClearBinding();

	/**
	 * Resets the read framebuffer to the default framebuffer (0).
	 */
	static void ClearBindingForReading();

	/**
	 * Resets the write framebuffer to the default framebuffer (0).
	 */
	static void ClearBindingForDrawing();

	/**
	 * Adds a texture to the framebuffer. Can only be done before creating
	 * (using Create()).
	 */
	void AddTexture(GLint internalFormat, GLenum format);

	/**
	 * Adds a texture to the framebuffer, where the caller can specify the
	 * attachment used. Can only be done before creating (using Create()).
	 */
	void AddTexture(GLint internalFormat, GLenum format, GLenum attachment);

	/**
	 * Adds a renderbuffer to the framebuffer. Can only be done before creating.
	 * (using Create()).
	 */
	void AddRenderbuffer(GLenum internalFormat, GLenum attachment);

	/**
	 * Actually creates this framebuffer. This must be called before use.
	 */
	void Create();

	/**
	 * Returns the width of the framebuffer
	 */
	unsigned Width() const;

	/**
	 * Returns the height of the framebuffer
	 */
	unsigned Height() const;

	/**
	 * Returns a vector of all the multisample textures attached to this
	 * framebuffer. If the framebuffer is not multisampled, this always returns
	 * and empty vector.
	 */
	const std::vector<GLTexture2DMultisample>& MultisampleTextures() const;

	/**
	 * Returns a vector of all textures attached to this framebuffer. If the
	 * framebuffer is multisampled, this always returns and empty vector.
	 */
	const std::vector<GLTexture2D>& Textures() const;

	/**
	 * Returns a vector of all the renderbuffers attached to this framebuffer.
	 */
	const std::vector<GLRenderbuffer>& Renderbuffers() const;

private:
	GLhandle _id;

	GLuint _width;
	GLuint _height;
	GLuint _samples;

	std::vector<GLTexture2D> _textures;
	std::vector<GLRenderbuffer> _renderbuffers;
	std::unordered_set<GLenum> _used_color_attachments;
	std::unordered_set<GLenum> _used_other_attachments;

	bool _is_multisampled;
	std::vector<GLTexture2DMultisample> _multisample_textures;

	std::vector<std::tuple<GLint, GLenum, GLenum>> _texture_add_info;
	std::vector<std::tuple<GLenum, GLenum>> _renderbuffer_add_info;

	bool _created;

private:
	static bool _IsColorAttachment(GLenum attachment);

};

}

#endif
