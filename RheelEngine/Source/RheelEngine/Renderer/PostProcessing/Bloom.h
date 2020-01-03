/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef BLOOM_H_
#define BLOOM_H_
#include "../../_common.h"

#include "../PostProcessingEffect.h"
#include "../OpenGL/GLShaderProgram.h"

namespace rheel {

class RE_API Bloom : public PostProcessingEffect {

public:
	Bloom(float thresholdStart, float thresholdEnd, float multiplier, float sigma, unsigned samples);

	const GLFramebuffer& Render(const GLFramebuffer& input) const override;

private:
	float _threshold_start;
	float _threshold_end;
	float _multiplier;
	std::vector<float> _kernel;

private:
	static GLShaderProgram& _BloomShader();
	static GLShaderProgram& _CombineShader();

	static std::unique_ptr<GLShaderProgram> _bloom_shader;
	static std::unique_ptr<GLShaderProgram> _combine_shader;

};

}

#endif
