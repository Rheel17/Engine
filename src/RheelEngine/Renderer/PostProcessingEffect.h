/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_POSTPROCESSINGEFFECT_H
#define RHEELENGINE_POSTPROCESSINGEFFECT_H
#include "../_common.h"

#include "OpenGL/Framebuffer.h"
#include "OpenGL/VertexArray.h"

namespace rheel {

class PostProcessingStack;

class PostProcessingEffect {
	friend class PostProcessingStack;

	struct RE_API screen_quad {
		screen_quad();
		gl::Buffer vbo;
		gl::VertexArray vao;
	};
	mutable pseudo_static_pointer<screen_quad> _screen_quad;

public:
	virtual ~PostProcessingEffect() = default;

	/**
	 * Renders this effect
	 */
	virtual const gl::Framebuffer& Render(const gl::Framebuffer& input) const = 0;

protected:
	/**
	 * Returns an unused temporary framebuffer index
	 */
	unsigned UnusedFramebufferIndex() const;

	/**
	 * Returns the index of a temporary framebuffer
	 */
	unsigned GetFramebufferIndex(const gl::Framebuffer& buffer) const;

	/**
	 * Returns the temporary framebuffer at an index
	 */
	gl::Framebuffer& Framebuffer(unsigned index) const;

	/**
	 * Sets the use flag of a temporary framebuffer
	 */
	void MarkFramebufferUse(unsigned index, bool flag) const;

private:
	PostProcessingStack *Stack_() const;

	PostProcessingStack *_stack;

protected:
	void DrawScreenQuad() const;

};

}

#endif
