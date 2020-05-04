/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PostProcessingEffect.h"

#include "PostProcessingStack.h"
#include "OpenGL/Context.h"

namespace rheel {

PostProcessingEffect::screen_quad::screen_quad() :
		vbo(gl::Buffer::Target::ARRAY) {

	gl::ContextScope cs;

	GLfloat triangles[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
	vbo.SetData(triangles, sizeof(triangles));
	vao.SetVertexAttributes<vec2>(vbo);
}

unsigned PostProcessingEffect::UnusedFramebufferIndex() const {
	return Stack_()->UnusedFramebufferIndex_();
}

unsigned PostProcessingEffect::GetFramebufferIndex(const gl::Framebuffer& buffer) const {
	return Stack_()->GetFramebufferIndex_(buffer);
}

gl::Framebuffer& PostProcessingEffect::Framebuffer(unsigned index) const {
	return Stack_()->Framebuffer_(index);
}

void PostProcessingEffect::MarkFramebufferUse(unsigned index, bool flag) const {
    Stack_()->MarkFramebufferUse_(index, flag);
}

PostProcessingStack *PostProcessingEffect::Stack_() const {
	if (!_stack) {
		throw std::runtime_error("Effect not part of a stack");
	}

	return _stack;
}

void PostProcessingEffect::DrawScreenQuad() const {
	_screen_quad->vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 6);
}

}
