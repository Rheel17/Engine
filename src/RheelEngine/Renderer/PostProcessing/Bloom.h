/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_BLOOM_H
#define RHEELENGINE_BLOOM_H
#include "../../_common.h"

#include "../PostProcessingEffect.h"
#include "../OpenGL/Program.h"

namespace rheel {

class RE_API Bloom : public PostProcessingEffect {

public:
	Bloom(float thresholdStart, float thresholdEnd, float multiplier, float sigma, unsigned samples);

	const gl::Framebuffer& Render(const gl::Framebuffer& input) const override;

private:
	float _threshold_start;
	float _threshold_end;
	float _multiplier;
	std::vector<float> _kernel;

private:
	static gl::Program& BloomShader_();
	static gl::Program& CombineShader_();

	static std::unique_ptr<gl::Program> _bloom_shader;
	static std::unique_ptr<gl::Program> _combine_shader;

};

}

#endif
