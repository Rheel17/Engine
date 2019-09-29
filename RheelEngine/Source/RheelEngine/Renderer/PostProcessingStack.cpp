/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "PostProcessingStack.h"

#include "../EngineResources.h"

namespace rheel {

std::unique_ptr<GLShaderProgram> PostProcessingStack::_bloom_shader;

PostProcessingStack::PostProcessingStack() :
		_temp_buffer_1(1, 1), _temp_buffer_2(1, 1) {

	_temp_buffer_1.AddTexture(GL_RGB, GL_RGB);
	_temp_buffer_1.Create();

	_temp_buffer_2.AddTexture(GL_RGB, GL_RGB);
	_temp_buffer_2.Create();
}

void PostProcessingStack::Render(const GLFramebuffer& input, const ivec2& pos, const ivec2& size) const {
	GLFramebuffer inputBuffer = input;

	// if the input is multisampled, blit it to the first temporary
	// framebuffer to result the multisampling.
	if (input.IsMultisampled()) {
		// setup the framebuffers
		_EnsureTempBuffersSize(size, 1);
		_temp_buffer_1.BindForDrawing();
		input.BindForReading();

		// blit
		glBlitFramebuffer(
				0, 0, input.Width(), input.Height(),
				0, 0, size.x, size.y,
				GL_COLOR_BUFFER_BIT, GL_NEAREST);

		inputBuffer = _temp_buffer_1;
		_temp_buffer_use = 1;
	}

	// render the post-processing stack
	if (_has_bloom) {
		inputBuffer = _RenderBloom(inputBuffer);
	}

	// resolve to the default frame buffer
	inputBuffer.BindForReading();
	GL::ClearFramebufferBinding(GL::FramebufferTarget::DRAW);

	glBlitFramebuffer(
			0, 0, inputBuffer.Width(), inputBuffer.Height(),
			pos.x, pos.y, pos.x + size.x, pos.y + size.y,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void PostProcessingStack::SetBloom(float thresholdStart, float thresholdEnd, float multiplier, unsigned level) {
	_has_bloom = true;
	_bloom = { thresholdStart, thresholdEnd, multiplier, level };
}

void PostProcessingStack::SetBloomOff() {
	_has_bloom = false;
	_bloom = Bloom {};
}

const PostProcessingStack::Bloom& PostProcessingStack::GetBloom() const {
	return _bloom;
}

void PostProcessingStack::_EnsureTempBuffersSize(const ivec2& size, unsigned index) const {
	if (index == 1) {
		if ((signed) _temp_buffer_1.Width() != size.x || (signed) _temp_buffer_1.Height() != size.y) {
			_temp_buffer_1 = _temp_buffer_1.ResizedCopy(size.x, size.y);
		}
	} else if (index == 2) {
		if ((signed) _temp_buffer_2.Width() != size.x || (signed) _temp_buffer_2.Height() != size.y) {
			_temp_buffer_2 = _temp_buffer_2.ResizedCopy(size.x, size.y);
		}
	}
}

const GLFramebuffer& PostProcessingStack::_RenderBloom(const GLFramebuffer& input) const {
	_EnsureTempBuffersSize({ input.Width(), input.Height() }, 1);
	_EnsureTempBuffersSize({ input.Width(), input.Height() }, 2);

	// blit the

	GLShaderProgram& bloomShader = _BloomShader();


	return _temp_buffer_2;
}

GLShaderProgram& PostProcessingStack::_BloomShader() {
	if (!_bloom_shader) {
		_bloom_shader = std::make_unique<GLShaderProgram>();
		_bloom_shader->AddShaderFromSource(GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_postprocessing_vert_glsl"));
		_bloom_shader->AddShaderFromSource(GLShaderProgram::FRAGMENT, EngineResources::PreprocessShader("Shaders_postprocssing_bloom_frag_glsl"));
	}

	return *_bloom_shader;
}

}
