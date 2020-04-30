/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Bloom.h"

#include "../../EngineResources.h"

namespace rheel {

Bloom::shaders::shaders() {
	bloom_shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_postprocessing_vert_glsl"));
	bloom_shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_postprocessing_bloom_frag_glsl"));
	bloom_shader.Link();

	combine_shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_postprocessing_vert_glsl"));
	combine_shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_postprocessing_combine_frag_glsl"));
	combine_shader.Link();
	combine_shader["inputTexture1"] = 1;
	combine_shader["inputTexture0"] = 0;
}

Bloom::Bloom(float thresholdStart, float thresholdEnd, float multiplier, float sigma, unsigned samples) :
		_threshold_start(thresholdStart),
		_threshold_end(thresholdEnd),
		_multiplier(multiplier) {

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

const gl::Framebuffer& Bloom::Render(const gl::Framebuffer& input) const {
	// setup the shader
	gl::Program& bloomShader = _shaders->bloom_shader;
	bloomShader["kernel"] = _kernel;
	bloomShader["kernelSize"] = (GLint) (_kernel.size() - 1);

	// setup the framebuffers
	unsigned idx1 = UnusedFramebufferIndex();
	gl::Framebuffer& buf1 = Framebuffer(idx1);
	MarkFramebufferUse(idx1, true);

	unsigned idx2 = UnusedFramebufferIndex();
	gl::Framebuffer& buf2 = Framebuffer(idx2);
	MarkFramebufferUse(idx2, true);

	// draw the horizontal blur
	input.GetTextureAttachment(0).Bind(0);
	bloomShader["vertical"] = (GLint) false;
	bloomShader["filterStart"] = _threshold_start;
	bloomShader["filterEnd"] = _threshold_end;

	buf1.BindForDrawing();
	DrawScreenQuad();

	// draw the vertical blur
	buf1.GetTextureAttachment(0).Bind(0);
	bloomShader["vertical"] = (GLint) true;
	bloomShader["filterStart"] = 0.0f;
	bloomShader["filterEnd"] = 0.0f;

	buf2.BindForDrawing();
	DrawScreenQuad();

	// combine the blurred image with the original to achieve bloom
	input.GetTextureAttachment(0).Bind(0);
	buf2.GetTextureAttachment(0).Bind(1);
	gl::Program& combineShader = _shaders->combine_shader;
	combineShader["factor0"] = 1.0f;
	combineShader["factor1"] = _multiplier;

	buf1.BindForDrawing();
	DrawScreenQuad();

	MarkFramebufferUse(idx2, false);
	return buf1;
}

}
