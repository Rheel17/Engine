/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Bloom.h"

#include "../../EngineResources.h"
#include "../OpenGL/Context.h"

namespace rheel {

Bloom::shaders::shaders() {
	gl::ContextScope cs;

	bloom_shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_postprocessing_vert_glsl"));
	bloom_shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_postprocessing_bloom_frag_glsl"));
	bloom_shader.Link();

	combine_shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_postprocessing_vert_glsl"));
	combine_shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_postprocessing_combine_frag_glsl"));
	combine_shader.Link();
	combine_shader["inputTexture1"] = 1;
	combine_shader["inputTexture0"] = 0;
}

Bloom::Bloom(float threshold_start, float threshold_end, float multiplier, float sigma, unsigned samples) :
		_threshold_start(threshold_start),
		_threshold_end(threshold_end),
		_multiplier(multiplier) {

	static const float sqrt_2_pi = sqrt(2.0f * M_PI);
	static const auto gaussian = [](float sigma, float x) {
		return std::exp(-(x * x) / (2 * sigma * sigma)) / (sqrt_2_pi * sigma);
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
	gl::Program& bloom_shader = _shaders->bloom_shader;
	bloom_shader["kernel"] = _kernel;
	bloom_shader["kernelSize"] = (GLint) (_kernel.size() - 1);

	// setup the framebuffers
	unsigned idx_1 = UnusedFramebufferIndex();
	gl::Framebuffer& buf_1 = Framebuffer(idx_1);
	MarkFramebufferUse(idx_1, true);

	unsigned idx_2 = UnusedFramebufferIndex();
	gl::Framebuffer& buf_2 = Framebuffer(idx_2);
	MarkFramebufferUse(idx_2, true);

	// draw the horizontal blur
	input.GetTextureAttachment(0).Bind(0);
	bloom_shader["vertical"] = (GLint) false;
	bloom_shader["filterStart"] = _threshold_start;
	bloom_shader["filterEnd"] = _threshold_end;

	buf_1.BindForDrawing();
	DrawScreenQuad();

	// draw the vertical blur
	buf_1.GetTextureAttachment(0).Bind(0);
	bloom_shader["vertical"] = (GLint) true;
	bloom_shader["filterStart"] = 0.0f;
	bloom_shader["filterEnd"] = 0.0f;

	buf_2.BindForDrawing();
	DrawScreenQuad();

	// combine the blurred image with the original to achieve bloom
	input.GetTextureAttachment(0).Bind(0);
	buf_2.GetTextureAttachment(0).Bind(1);
	gl::Program& combine_shader = _shaders->combine_shader;
	combine_shader["factor0"] = 1.0f;
	combine_shader["factor1"] = _multiplier;

	buf_1.BindForDrawing();
	DrawScreenQuad();

	MarkFramebufferUse(idx_2, false);
	return buf_1;
}

}
