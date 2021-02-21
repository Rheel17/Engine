/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PostProcessingStack.h"
#include "OpenGL/Context.h"

namespace rheel {

PostProcessingStack::PostProcessingStack() :
		_temp_buffers({
				std::make_pair<gl::Framebuffer>(gl::Framebuffer(1, 1), false),
				std::make_pair<gl::Framebuffer>(gl::Framebuffer(1, 1), false),
				std::make_pair<gl::Framebuffer>(gl::Framebuffer(1, 1), false)
		}) {

	{
		gl::ContextScope cs;

		for (auto& b : _temp_buffers) {
			b.first.AttachTexture(gl::InternalFormat::RGBA, gl::Format::RGBA, 0);
			b.first.SetDrawBuffers({ 0 });
		}
	}
}

void PostProcessingStack::Render(const gl::Framebuffer& input, const ivec2& pos, const ivec2& size) const {
	gl::Context::Current().Push();

	// set size
	_width = size.x;
	_height = size.y;

	// reset temporary framebuffer usage
	for (auto& temp_buffer : _temp_buffers) {
		temp_buffer.second = false;
	}

	// resolve the input as first buffer
	std::reference_wrapper<const gl::Framebuffer> input_buffer =
			std::ref(_resolve_input(input));

	// helper function for rendering a single effect
	static const auto pp = [&input_buffer, this](const auto& effect) {
		if (effect) {
			const gl::Framebuffer& prev = input_buffer.get();
			input_buffer = std::ref(effect->Render(input_buffer.get()));

			if (&input_buffer.get() != &prev) {
				_mark_framebuffer_use(_get_framebuffer_index(prev), false);
			}
		}
	};

	// render the post-processing stack
	pp(_bloom);

	// resolve to the default frame buffer
	gl::Context::Current().Pop();
	gl::Context::Current().Push();

	input_buffer.get().Blit({ 0, 0, _width, _height }, { pos.x, pos.y, size.x, size.y }, gl::Framebuffer::BitField::COLOR);

	gl::Context::Current().Pop();
}

const gl::Framebuffer& PostProcessingStack::_resolve_input(const gl::Framebuffer& input) const {
	auto type = input.GetAttachmentType(0);

	// if the input is multisampled, blit it to the first temporary
	// framebuffer to result the multisampling.
	if (type == gl::Framebuffer::AttachmentType::TEXTURE_MULTISAMPLE) {
		// setup the framebuffers
		unsigned index = _unused_framebuffer_index();
		gl::Framebuffer& tmp = _framebuffer(index);
		_mark_framebuffer_use(index, true);

		// blit
		{
			gl::ContextScope cs;

			tmp.BindForDrawing();
			input.Blit(
					{ 0, 0, input.GetViewportWidth(), input.GetViewportHeight() },
					{ 0, 0, _width, _height },
					gl::Framebuffer::BitField::COLOR);

		}
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

unsigned PostProcessingStack::_unused_framebuffer_index() const {
	for (unsigned i = 0; i < _temp_buffers.size(); i++) {
		if (!_temp_buffers[i].second) {
			return i;
		}
	}

	throw std::runtime_error("Out of temp buffers!");
}

unsigned PostProcessingStack::_get_framebuffer_index(const gl::Framebuffer& buffer) const {
	for (unsigned i = 0; i < _temp_buffers.size(); i++) {
		if (&_temp_buffers[i].first == &buffer) {
			return i;
		}
	}

	throw std::runtime_error("");
}

gl::Framebuffer& PostProcessingStack::_framebuffer(unsigned index) const {
	gl::Framebuffer& fbo = _temp_buffers[index].first;

	if (fbo.GetViewportWidth() != _width || fbo.GetViewportHeight() != _height) {
		_temp_buffers[index].first = gl::Framebuffer(_temp_buffers[index].first, _width, _height);
		return _temp_buffers[index].first;
	}

	return fbo;
}

void PostProcessingStack::_mark_framebuffer_use(unsigned index, bool flag) const {
	_temp_buffers[index].second = flag;
}

}
