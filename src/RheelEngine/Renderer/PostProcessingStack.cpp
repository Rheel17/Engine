/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PostProcessingStack.h"
#include "OpenGL/State.h"

namespace rheel {

PostProcessingStack::PostProcessingStack() :
		_temp_buffers({
				std::make_pair<GL::Framebuffer>(GL::Framebuffer(1, 1), false),
				std::make_pair<GL::Framebuffer>(GL::Framebuffer(1, 1), false),
				std::make_pair<GL::Framebuffer>(GL::Framebuffer(1, 1), false)
		}) {

	for (auto& b : _temp_buffers) {
		b.first.AttachTexture(GL::InternalFormat::RGBA, GL::Format::RGBA, 0);
		b.first.SetDrawBuffers({ 0 });
	}
}

void PostProcessingStack::Render(const GL::Framebuffer& input, const ivec2& pos, const ivec2& size) const {
	GL::State::Push();

	// set size
	_width = size.x;
	_height = size.y;

	// reset temporary framebuffer usage
	for (auto& _temp_buffer : _temp_buffers) {
		_temp_buffer.second = false;
	}

	// resolve the input as first buffer
	std::reference_wrapper<const GL::Framebuffer> inputBuffer =
			std::ref(_ResolveInput(input));

	// helper function for rendering a single effect
	static const auto pp = [&inputBuffer, this](const auto& effect) {
		if (effect) {
			const GL::Framebuffer& prev = inputBuffer.get();
			inputBuffer = std::ref(effect->Render(inputBuffer.get()));

			if (&inputBuffer.get() != &prev) {
				_MarkFramebufferUse(_GetFramebufferIndex(prev), false);
			}
		}
	};

	// render the post-processing stack
	pp(_bloom);

	// resolve to the default frame buffer
	GL::State::Pop();
	GL::State::Push();

	inputBuffer.get().Blit({ 0, 0, _width, _height }, { pos.x, pos.y, size.x, size.y }, GL::Framebuffer::BitField::COLOR);

	GL::State::Pop();
}

const GL::Framebuffer& PostProcessingStack::_ResolveInput(const GL::Framebuffer& input) const {
	auto type = input.GetAttachmentType(0);

	// if the input is multisampled, blit it to the first temporary
	// framebuffer to result the multisampling.
	if (type == GL::Framebuffer::AttachmentType::TEXTURE_MULTISAMPLE) {
		// setup the framebuffers
		unsigned index = _UnusedFramebufferIndex();
		GL::Framebuffer& tmp = _Framebuffer(index);
		_MarkFramebufferUse(index, true);

		// blit
		GL::State::Push();

		tmp.BindForDrawing();
		input.Blit({ 0, 0, input.GetViewportWidth(), input.GetViewportHeight() },
				   { 0, 0, _width, _height },
				   GL::Framebuffer::BitField::COLOR);

		GL::State::Pop();

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

unsigned PostProcessingStack::_GetFramebufferIndex(const GL::Framebuffer& buffer) const {
	for (unsigned i = 0; i < _temp_buffers.size(); i++) {
		if (&_temp_buffers[i].first == &buffer) {
			return i;
		}
	}

	throw std::runtime_error("");
}

GL::Framebuffer& PostProcessingStack::_Framebuffer(unsigned index) const {
	GL::Framebuffer& fbo = _temp_buffers[index].first;

	if (fbo.GetViewportWidth() != _width || fbo.GetViewportHeight() != _height) {
		_temp_buffers[index].first = GL::Framebuffer(_temp_buffers[index].first, _width, _height);
		return _temp_buffers[index].first;
	}

	return fbo;
}

void PostProcessingStack::_MarkFramebufferUse(unsigned index, bool flag) const {
	_temp_buffers[index].second = flag;
}

}
