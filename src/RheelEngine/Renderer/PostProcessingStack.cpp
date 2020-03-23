/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PostProcessingStack.h"

namespace rheel {

PostProcessingStack::PostProcessingStack() :
		_temp_buffers({
				std::make_pair<_GLFramebuffer>(_GLFramebuffer(1, 1), false),
				std::make_pair<_GLFramebuffer>(_GLFramebuffer(1, 1), false),
				std::make_pair<_GLFramebuffer>(_GLFramebuffer(1, 1), false)
		}) {

	for (auto& b : _temp_buffers) {
		b.first.AddTexture(GL_RGB, GL_RGB);
		b.first.Create();
	}
}

void PostProcessingStack::Render(const _GLFramebuffer& input, const ivec2& pos, const ivec2& size) const {
	// set size
	_width = size.x;
	_height = size.y;

	// reset temporary framebuffer usage
	for (unsigned i = 0; i < _temp_buffers.size(); i++) {
		_temp_buffers[i].second = false;
	}

	// resolve the input as first buffer
	std::reference_wrapper<const _GLFramebuffer> inputBuffer =
			std::ref(_ResolveInput(input));

	// helper function for rendering a single effect
	static const auto pp = [&inputBuffer, this](const auto& effect) {
		if (effect) {
			const _GLFramebuffer& prev = inputBuffer.get();
			inputBuffer = std::ref(effect->Render(inputBuffer.get()));

			if (&inputBuffer.get() != &prev) {
				_MarkFramebufferUse(_GetFramebufferIndex(prev), false);
			}
		}
	};

	// render the post-processing stack
	pp(_bloom);

	// resolve to the default frame buffer
	inputBuffer.get().BindForReading();
	_GL::ClearFramebufferBinding(_GL::FramebufferTarget::DRAW);

	glBlitFramebuffer(
			0, 0, _width, _height,
			pos.x, pos.y, pos.x + size.x, pos.y + size.y,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

const _GLFramebuffer& PostProcessingStack::_ResolveInput(const _GLFramebuffer& input) const {
	// if the input is multisampled, blit it to the first temporary
	// framebuffer to result the multisampling.
	if (input.IsMultisampled()) {

		// setup the framebuffers
		unsigned index = _UnusedFramebufferIndex();
		_GLFramebuffer& tmp = _Framebuffer(index);
		_MarkFramebufferUse(index, true);

		tmp.BindForDrawing();
		input.BindForReading();

		// blit
		glBlitFramebuffer(
				0, 0, input.Width(), input.Height(),
				0, 0, _width, _height,
				GL_COLOR_BUFFER_BIT, GL_NEAREST);

		return tmp;
	} else {
		return input;
	}
}

void PostProcessingStack::SetBloom(Bloom bloom) {
	_bloom = std::move(bloom);
	_bloom->_stack = this;
}

void PostProcessingStack::ClearBloom() {
	_bloom.reset();
}

unsigned PostProcessingStack::_UnusedFramebufferIndex() const {
	for (unsigned i = 0; i < _temp_buffers.size(); i++) {
		if (!_temp_buffers[i].second) {
			return i;
		}
	}

	throw std::runtime_error("Out of temp buffers!");
}

unsigned PostProcessingStack::_GetFramebufferIndex(const _GLFramebuffer& buffer) const {
	for (unsigned i = 0; i < _temp_buffers.size(); i++) {
		if (&_temp_buffers[i].first == &buffer) {
			return i;
		}
	}

	throw std::runtime_error("");
}

_GLFramebuffer& PostProcessingStack::_Framebuffer(unsigned index) const {
	_GLFramebuffer& fbo = _temp_buffers[index].first;

	if (fbo.Width() != _width || fbo.Height() != _height) {
		_temp_buffers[index].first = _temp_buffers[index].first.ResizedCopy(_width, _height);
		return _temp_buffers[index].first;
	}

	return fbo;
}

void PostProcessingStack::_MarkFramebufferUse(unsigned index, bool flag) const {
	_temp_buffers[index].second = flag;
}

}
