/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PostProcessingEffect.h"

#include "PostProcessingStack.h"

namespace rheel {

std::unique_ptr<_GLBuffer> PostProcessingEffect::_screen_quad_buffer;
std::unique_ptr<_GLVertexArray> PostProcessingEffect::_screen_quad;

unsigned PostProcessingEffect::_UnusedFramebufferIndex() const {
	return _Stack()->_UnusedFramebufferIndex();
}

unsigned PostProcessingEffect::_GetFramebufferIndex(const _GLFramebuffer& buffer) const {
	return _Stack()->_GetFramebufferIndex(buffer);
}

_GLFramebuffer& PostProcessingEffect::_Framebuffer(unsigned index) const {
	return _Stack()->_Framebuffer(index);
}

void PostProcessingEffect::_MarkFramebufferUse(unsigned index, bool flag) const {
	_Stack()->_MarkFramebufferUse(index, flag);
}

PostProcessingStack *PostProcessingEffect::_Stack() const {
	if (!_stack) {
		throw std::runtime_error("Effect not part of a stack");
	}

	return _stack;
}

void PostProcessingEffect::_DrawScreenQuad() {
	if (!_screen_quad) {
		GLfloat triangles[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
		_screen_quad_buffer = std::make_unique<_GLBuffer>(_GL::BufferTarget::ARRAY);
		_screen_quad_buffer->SetData(triangles, sizeof(triangles));

		_screen_quad = std::make_unique<_GLVertexArray>();
		_screen_quad->SetVertexAttributes<vec2>(*_screen_quad_buffer);
	}

	_screen_quad->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

}
