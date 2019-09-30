/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "PostProcessingStack.h"

#include "../EngineResources.h"

namespace rheel {

std::unique_ptr<GLShaderProgram> PostProcessingStack::_bloom_shader;
std::unique_ptr<GLShaderProgram> PostProcessingStack::_combine_shader;
std::unique_ptr<GLBuffer> PostProcessingStack::_screen_quad_buffer;
std::unique_ptr<GLVertexArray> PostProcessingStack::_screen_quad;

PostProcessingStack::PostProcessingStack() :
		_temp_buffer_1(1, 1), _temp_buffer_2(1, 1), _temp_buffer_3(1, 1) {

	_temp_buffer_1.AddTexture(GL_RGB, GL_RGB);
	_temp_buffer_1.Create();

	_temp_buffer_2.AddTexture(GL_RGB, GL_RGB);
	_temp_buffer_2.Create();

	_temp_buffer_3.AddTexture(GL_RGB, GL_RGB);
	_temp_buffer_3.Create();
}

void PostProcessingStack::Render(const GLFramebuffer& input, const ivec2& pos, const ivec2& size) const {
	// reset temp buffers
	for (unsigned i = 0; i < 3; i++) {
		_temp_buffer_use[i] = false;
	}

	GLFramebuffer inputBuffer = input;
	unsigned inputIndex = 0;

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
		inputIndex = 1;
		_temp_buffer_use[0] = true;
	}

	// render the post-processing stack
	if (_has_bloom) {
		inputBuffer = _RenderBloom(inputBuffer, inputIndex);
	}

	// resolve to the default frame buffer
	inputBuffer.BindForReading();
	GL::ClearFramebufferBinding(GL::FramebufferTarget::DRAW);

	glBlitFramebuffer(
			0, 0, inputBuffer.Width(), inputBuffer.Height(),
			pos.x, pos.y, pos.x + size.x, pos.y + size.y,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void PostProcessingStack::SetBloom(float thresholdStart, float thresholdEnd, float multiplier, float sigma, unsigned samples) {
	_has_bloom = true;
	_bloom = { thresholdStart, thresholdEnd, multiplier };

	static const float sqrt2pi = sqrt(2.0f * M_PI);
	static const auto gaussian = [](float sigma, float x) {
		return std::exp(-(x * x) / (2 * sigma * sigma)) / (sqrt2pi * sigma);
	};

	float sum = gaussian(sigma, 0);
	_bloom.kernel.reserve(samples + 1);
	_bloom.kernel.push_back(sum);

	for (unsigned i = 0; i < samples; i++) {
		float value = gaussian(sigma, i + 1);
		sum += 2 * value;
		_bloom.kernel.push_back(value);
	}

	for (unsigned i = 0; i <= samples; i++) {
		_bloom.kernel[i] /= sum;
	}
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
	} else if (index == 3) {
		if ((signed) _temp_buffer_3.Width() != size.x || (signed) _temp_buffer_3.Height() != size.y) {
			_temp_buffer_3 = _temp_buffer_3.ResizedCopy(size.x, size.y);
		}
	}
}

GLFramebuffer& PostProcessingStack::_GetFramebuffer(unsigned index) const {
	if (index == 1) {
		return _temp_buffer_1;
	} else if (index == 2) {
		return _temp_buffer_2;
	} else {
		assert(index == 3);
		return _temp_buffer_3;
	}
}

unsigned PostProcessingStack::_NextUnusedFramebufferIndex() const {
	if (!_temp_buffer_use[0]) {
		return 1;
	} else if (!_temp_buffer_use[1]) {
		return 2;
	} else if (!_temp_buffer_use[2]) {
		return 3;
	}

	throw std::runtime_error("Out of temp buffers!");
}

const GLFramebuffer& PostProcessingStack::_RenderBloom(const GLFramebuffer& input, unsigned& inputIndex) const {
	// setup the shader
	GLShaderProgram& bloomShader = _BloomShader();
	bloomShader["kernel"] = _bloom.kernel;
	bloomShader["kernelSize"] = (GLint) (_bloom.kernel.size() - 1);

	// setup the framebuffers
	unsigned idx1 = _NextUnusedFramebufferIndex();
	_temp_buffer_use[idx1 - 1] = true;

	unsigned idx2 = _NextUnusedFramebufferIndex();
	_temp_buffer_use[idx2 - 1] = true;

	_EnsureTempBuffersSize({ input.Width(), input.Height() }, idx1);
	_EnsureTempBuffersSize({ input.Width(), input.Height() }, idx2);

	GLFramebuffer& fbo1 = _GetFramebuffer(idx1);
	GLFramebuffer& fbo2 = _GetFramebuffer(idx2);

	// draw the horizontal blur
	input.Textures()[0].Bind(0);
	bloomShader["vertical"] = (GLint) false;
	bloomShader["filterStart"] = _bloom.threshold_start;
	bloomShader["filterEnd"] = _bloom.threshold_end;

	fbo1.Bind();
	_DrawScreenQuad();

	// draw the vertical blur
	fbo1.Textures()[0].Bind(0);
	bloomShader["vertical"] = (GLint) true;
	bloomShader["filterStart"] = 0.0f;
	bloomShader["filterEnd"] = 0.0f;

	fbo2.Bind();
	_DrawScreenQuad();

	// combine the blurred image with the original to achieve bloom
	input.Textures()[0].Bind(0);
	fbo2.Textures()[0].Bind(1);
	GLShaderProgram& combineShader = _CombineShader();
	combineShader["factor0"] = 1.0f;
	combineShader["factor1"] = _bloom.multiplier;

	fbo1.Bind();
	_DrawScreenQuad();

	if (inputIndex > 0) {
		_temp_buffer_use[inputIndex - 1] = false;
	}

	_temp_buffer_use[idx2] = false;
	inputIndex = idx1;

	return fbo1;
}

GLShaderProgram& PostProcessingStack::_BloomShader() {
	if (!_bloom_shader) {
		_bloom_shader = std::make_unique<GLShaderProgram>();
		_bloom_shader->AddShaderFromSource(GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_postprocessing_vert_glsl"));
		_bloom_shader->AddShaderFromSource(GLShaderProgram::FRAGMENT, EngineResources::PreprocessShader("Shaders_postprocessing_bloom_frag_glsl"));
		_bloom_shader->Link();
	}

	return *_bloom_shader;
}

GLShaderProgram& PostProcessingStack::_CombineShader() {
	if (!_combine_shader) {
		_combine_shader = std::make_unique<GLShaderProgram>();
		_combine_shader->AddShaderFromSource(GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_postprocessing_vert_glsl"));
		_combine_shader->AddShaderFromSource(GLShaderProgram::FRAGMENT, EngineResources::PreprocessShader("Shaders_postprocessing_combine_frag_glsl"));
		_combine_shader->Link();
		_combine_shader->GetUniform("inputTexture0") = 0;
		_combine_shader->GetUniform("inputTexture1") = 1;
	}

	return *_combine_shader;
}

void PostProcessingStack::_DrawScreenQuad() {
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
