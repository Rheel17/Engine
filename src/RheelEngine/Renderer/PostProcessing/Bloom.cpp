/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Bloom.h"

#include "../../EngineResources.h"

namespace rheel {

std::unique_ptr<_GLShaderProgram> Bloom::_bloom_shader;
std::unique_ptr<_GLShaderProgram> Bloom::_combine_shader;

Bloom::Bloom(float thresholdStart, float thresholdEnd, float multiplier, float sigma, unsigned samples) :
		_threshold_start(thresholdStart), _threshold_end(thresholdEnd), _multiplier(multiplier) {

	static const float sqrt2pi = sqrt(2.0f * M_PI);
	static const auto gaussian = [](float sigma, float x) {
		return std::exp(-(x * x) / (2 * sigma * sigma)) / (sqrt2pi * sigma);
	};

	float sum = gaussian(sigma, 0);
	_kernel.reserve(samples + 1);
	_kernel.push_back(sum);

	for (unsigned i = 0; i < samples; i++) {
		float value = gaussian(sigma, i + 1);
		sum += 2 * value;
		_kernel.push_back(value);
	}

	for (unsigned i = 0; i <= samples; i++) {
		_kernel[i] /= sum;
	}
}

const _GLFramebuffer& Bloom::Render(const _GLFramebuffer& input) const {
	// setup the shader
	_GLShaderProgram& bloomShader = _BloomShader();
	bloomShader["kernel"] = _kernel;
	bloomShader["kernelSize"] = (GLint) (_kernel.size() - 1);

	// setup the framebuffers
	unsigned idx1 = _UnusedFramebufferIndex();
	_GLFramebuffer& buf1 = _Framebuffer(idx1);
	_MarkFramebufferUse(idx1, true);

	unsigned idx2 = _UnusedFramebufferIndex();
	_GLFramebuffer& buf2 = _Framebuffer(idx2);
	_MarkFramebufferUse(idx2, true);

	// draw the horizontal blur
	input.Textures()[0].Bind(0);
	bloomShader["vertical"] = (GLint) false;
	bloomShader["filterStart"] = _threshold_start;
	bloomShader["filterEnd"] = _threshold_end;

	buf1.Bind();
	_DrawScreenQuad();

	// draw the vertical blur
	buf1.Textures()[0].Bind(0);
	bloomShader["vertical"] = (GLint) true;
	bloomShader["filterStart"] = 0.0f;
	bloomShader["filterEnd"] = 0.0f;

	buf2.Bind();
	_DrawScreenQuad();

	// combine the blurred image with the original to achieve bloom
	input.Textures()[0].Bind(0);
	buf2.Textures()[0].Bind(1);
	_GLShaderProgram& combineShader = _CombineShader();
	combineShader["factor0"] = 1.0f;
	combineShader["factor1"] = _multiplier;

	buf1.Bind();
	_DrawScreenQuad();

	_MarkFramebufferUse(idx2, false);
	return buf1;
}

_GLShaderProgram& Bloom::_BloomShader() {
	if (!_bloom_shader) {
		_bloom_shader = std::make_unique<_GLShaderProgram>();
		_bloom_shader->AddShaderFromSource(_GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_postprocessing_vert_glsl"));
		_bloom_shader->AddShaderFromSource(_GLShaderProgram::FRAGMENT, EngineResources::PreprocessShader("Shaders_postprocessing_bloom_frag_glsl"));
		_bloom_shader->Link();
	}

	return *_bloom_shader;
}

_GLShaderProgram& Bloom::_CombineShader() {
	if (!_combine_shader) {
		_combine_shader = std::make_unique<_GLShaderProgram>();
		_combine_shader->AddShaderFromSource(_GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_postprocessing_vert_glsl"));
		_combine_shader->AddShaderFromSource(_GLShaderProgram::FRAGMENT, EngineResources::PreprocessShader("Shaders_postprocessing_combine_frag_glsl"));
		_combine_shader->Link();
		_combine_shader->GetUniform("inputTexture0") = 0;
		_combine_shader->GetUniform("inputTexture1") = 1;
	}

	return *_combine_shader;
}

}
