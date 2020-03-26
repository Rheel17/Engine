/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef POSTPROCESSINGEFFECT_H_
#define POSTPROCESSINGEFFECT_H_
#include "../_common.h"

#include "OpenGL/Framebuffer.h"
#include "OpenGL/VertexArray.h"

namespace rheel {

class PostProcessingStack;

class PostProcessingEffect {
	friend class PostProcessingStack;

public:
	virtual ~PostProcessingEffect() = default;

	/**
	 * Renders this effect
	 */
	virtual const GL::Framebuffer& Render(const GL::Framebuffer& input) const = 0;

protected:
	/**
	 * Returns an unused temporary framebuffer index
	 */
	unsigned _UnusedFramebufferIndex() const;

	/**
	 * Returns the index of a temporary framebuffer
	 */
	unsigned _GetFramebufferIndex(const GL::Framebuffer& buffer) const;

	/**
	 * Returns the temporary framebuffer at an index
	 */
	GL::Framebuffer& _Framebuffer(unsigned index) const;

	/**
	 * Sets the use flag of a temporary framebuffer
	 */
	void _MarkFramebufferUse(unsigned index, bool flag) const;

private:
	PostProcessingStack *_Stack() const;

	PostProcessingStack *_stack;

protected:
	static void _DrawScreenQuad();

	static std::unique_ptr<GL::Buffer> _screen_quad_buffer;
	static std::unique_ptr<GL::VertexArray> _screen_quad;

};

}

#endif
