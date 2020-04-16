/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PostProcessingEffect.h"

#include "PostProcessingStack.h"

namespace rheel {

std::unique_ptr<gl::Buffer> PostProcessingEffect::_screen_quad_buffer;
std::unique_ptr<gl::VertexArray> PostProcessingEffect::_screen_quad;

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

void PostProcessingEffect::DrawScreenQuad() {
	if (!_screen_quad) {
		GLfloat triangles[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
        _screen_quad_buffer = std::make_unique<gl::Buffer>(gl::Buffer::Target::ARRAY);
		_screen_quad_buffer->SetData(triangles, sizeof(triangles));

        _screen_quad = std::make_unique<gl::VertexArray>();
		_screen_quad->SetVertexAttributes<vec2>(*_screen_quad_buffer);
	}

	_screen_quad->DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 6);
}

}
