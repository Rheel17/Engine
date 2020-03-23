/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef BLOOM_H_
#define BLOOM_H_
#include "../../_common.h"

#include "../PostProcessingEffect.h"
#include "../_OpenGL/_GLShaderProgram.h"

namespace rheel {

class RE_API Bloom : public PostProcessingEffect {

public:
	Bloom(float thresholdStart, float thresholdEnd, float multiplier, float sigma, unsigned samples);

	const _GLFramebuffer& Render(const _GLFramebuffer& input) const override;

private:
	float _threshold_start;
	float _threshold_end;
	float _multiplier;
	std::vector<float> _kernel;

private:
	static _GLShaderProgram& _BloomShader();
	static _GLShaderProgram& _CombineShader();

	static std::unique_ptr<_GLShaderProgram> _bloom_shader;
	static std::unique_ptr<_GLShaderProgram> _combine_shader;

};

}

#endif
