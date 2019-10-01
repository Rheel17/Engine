/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "PostProcessingEffect.h"

#include "PostProcessingStack.h"

namespace rheel {

std::unique_ptr<GLBuffer> PostProcessingEffect::_screen_quad_buffer;
std::unique_ptr<GLVertexArray> PostProcessingEffect::_screen_quad;

unsigned PostProcessingEffect::_UnusedFramebufferIndex() const {
	return _Stack()->_UnusedFramebufferIndex();
}

unsigned PostProcessingEffect::_GetFramebufferIndex(const GLFramebuffer& buffer) const {
	return _Stack()->_GetFramebufferIndex(buffer);
}

GLFramebuffer& PostProcessingEffect::_Framebuffer(unsigned index) const {
	return _Stack()->_Framebuffer(index);
}

void PostProcessingEffect::_MarkFramebufferUse(unsigned index, bool flag) const {
	_Stack()->_MarkFramebufferUse(index, flag);
}

const GLFramebuffer& PostProcessingEffect::_InputBuffer() const {
	return *_Stack()->_input_buffer;
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
		_screen_quad_buffer = std::make_unique<GLBuffer>(GL::BufferTarget::ARRAY);
		_screen_quad_buffer->SetData(triangles, sizeof(triangles));

		_screen_quad = std::make_unique<GLVertexArray>();
		_screen_quad->SetVertexAttributes<vec2>(*_screen_quad_buffer);
	}

	_screen_quad->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

}
