/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_STATEBINDINGS_H
#define RHEELENGINE_STATEBINDINGS_H
#include "../../_common.h"

#include "Buffer.h"
#include "Framebuffer.h"

namespace rheel {
class Engine;
}

namespace rheel::gl {

class RE_API StateBindings {
	friend class rheel::Engine;

public:
	StateBindings();
	explicit StateBindings(const StateBindings* parent);

	void BindBuffer(Buffer::Target target, GLuint name);
	void BindFramebuffer(Framebuffer::Target target, GLuint name, unsigned width, unsigned height);
	void BindRenderbuffer(GLuint name);
	void BindTexture(unsigned unit, Texture::Target target, GLuint name);
	void BindVertexArray(GLuint name);
	void UseProgram(GLuint handle);

	void ResetChanges();

private:
	GLuint GetBuffer_(Buffer::Target target) const;
	GLuint GetFramebuffer_(Framebuffer::Target target) const;
	GLuint GetRenderbuffer_() const;
	GLuint GetTexture_(unsigned unit, Texture::Target target) const;
	GLuint GetVertexArray_() const;
	GLuint GetProgram_() const;

	void SetViewport_(uvec2 dim);
	uvec2 GetViewport_() const;

	std::unordered_map<Buffer::Target, GLuint> _buffer_changes;
	std::unordered_map<Framebuffer::Target, GLuint> _framebuffer_changes;
	std::optional<GLuint> _renderbuffer_change;
	std::unordered_map<std::pair<GLuint, Texture::Target>, GLuint> _texture_changes;
	std::optional<GLuint> _vertex_array_change;
	std::optional<GLuint> _program_change;

	std::optional<uvec2> _viewport_change;

	const StateBindings* _parent;

private:
	static void SetActiveTextureUnit_(unsigned unit);

	static unsigned _active_texture_unit;

};

}

#endif
