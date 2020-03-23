/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_STATEBINDINGS_H
#define RHEELENGINE_GL_STATEBINDINGS_H
#include "../../_common.h"

#include "Buffer.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Program.h"

namespace rheel {
	class Engine;
}

namespace rheel::GL {

class RE_API StateBindings {
	friend class rheel::Engine;
	friend class Uniform;

public:
	StateBindings();
	explicit StateBindings(const StateBindings *parent);

	void BindBuffer(const Buffer& buffer);
	void BindFramebuffer(Framebuffer::Target target, const Framebuffer& framebuffer);
	void BindRenderbuffer(const Renderbuffer& renderbuffer);
	void BindTexture(unsigned unit, const Texture& texture);
	void BindVertexArray(const VertexArray& vertexArray);
	void UseProgram(const Program& program);

	void ResetChanges();

private:
	void _UseProgram(GLuint programHandle);

	GLuint _GetBuffer(Buffer::Target target) const;
	GLuint _GetFramebuffer(Framebuffer::Target target) const;
	GLuint _GetRenderbuffer() const;
	GLuint _GetTexture(unsigned unit, Texture::Target target) const;
	GLuint _GetVertexArray() const;
	GLuint _GetProgram() const;

	void _SetViewport(uvec2 dim);
	uvec2 _GetViewport() const;

	std::unordered_map<Buffer::Target, GLuint> _buffer_changes;
	std::unordered_map<Framebuffer::Target, GLuint> _framebuffer_changes;
	std::optional<GLuint> _renderbuffer_change;
	std::unordered_map<std::pair<GLuint, Texture::Target>, GLuint> _texture_changes;
	std::optional<GLuint> _vertex_array_change;
	std::optional<GLuint> _program_change;

	std::optional<uvec2> _viewport_change;

	const StateBindings *_parent;

private:
	static void _SetDefaultViewport(unsigned width, unsigned height);

	static uvec2 _default_viewport;

};

}

#endif
