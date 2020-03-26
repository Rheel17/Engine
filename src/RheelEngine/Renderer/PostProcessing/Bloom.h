/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef BLOOM_H_
#define BLOOM_H_
#include "../../_common.h"

#include "../PostProcessingEffect.h"
#include "../OpenGL/Program.h"

namespace rheel {

class RE_API Bloom : public PostProcessingEffect {

public:
	Bloom(float thresholdStart, float thresholdEnd, float multiplier, float sigma, unsigned samples);

	const GL::Framebuffer& Render(const GL::Framebuffer& input) const override;

private:
	float _threshold_start;
	float _threshold_end;
	float _multiplier;
	std::vector<float> _kernel;

private:
	static GL::Program& _BloomShader();
	static GL::Program& _CombineShader();

	static std::unique_ptr<GL::Program> _bloom_shader;
	static std::unique_ptr<GL::Program> _combine_shader;

};

}

#endif
