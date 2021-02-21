/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CONTEXTBINDINGS_H
#define RHEELENGINE_CONTEXTBINDINGS_H
#include "../../_common.h"

#include "Buffer.h"
#include "Framebuffer.h"

namespace rheel::gl {

class Context;

class RE_API ContextBindings {

public:
	explicit ContextBindings(Context& context);
	explicit ContextBindings(const ContextBindings* parent);

	void BindBuffer(Buffer::Target target, GLuint name);
	void BindFramebuffer(Framebuffer::Target target, GLuint name, unsigned width, unsigned height);
	void BindRenderbuffer(GLuint name);
	void BindTexture(unsigned unit, Texture::Target target, GLuint name);
	void BindVertexArray(GLuint name);
	void UseProgram(GLuint handle);

	void ResetChanges();

private:
	GLuint _get_buffer(Buffer::Target target) const;
	GLuint _get_framebuffer(Framebuffer::Target target) const;
	GLuint _get_renderbuffer() const;
	GLuint _get_texture(unsigned unit, Texture::Target target) const;
	GLuint _get_vertex_array() const;
	GLuint _get_program() const;

	void _set_viewport(uvec2 dim);
	uvec2 _get_viewport() const;

	Context& _context;

	std::unordered_map<Buffer::Target, GLuint> _buffer_changes;
	std::unordered_map<Framebuffer::Target, GLuint> _framebuffer_changes;
	std::optional<GLuint> _renderbuffer_change;
	std::unordered_map<std::pair<GLuint, Texture::Target>, GLuint> _texture_changes;
	std::optional<GLuint> _vertex_array_change;
	std::optional<GLuint> _program_change;

	std::optional<uvec2> _viewport_change;

	const ContextBindings* _parent;

};

}

#endif
