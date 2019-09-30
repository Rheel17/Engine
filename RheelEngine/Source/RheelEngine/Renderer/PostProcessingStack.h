/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef POSTPROCESSINGSTACK_H_
#define POSTPROCESSINGSTACK_H_
#include "../_common.h"

#include "OpenGL/GLFramebuffer.h"
#include "OpenGL/GLShaderProgram.h"
#include "OpenGL/GLVertexArray.h"

namespace rheel {

class RE_API PostProcessingStack {

public:
	struct Bloom {
		float threshold_start;
		float threshold_end;
		float multiplier;
		std::vector<float> kernel;
	};

public:
	PostProcessingStack();

	void Render(const GLFramebuffer& input, const ivec2& pos, const ivec2& size) const;

	void SetBloom(float thresholdStart, float thresholdEnd, float multiplier, float sigma, unsigned samples);
	void SetBloomOff();
	const Bloom& GetBloom() const;

private:
	void _EnsureTempBuffersSize(const ivec2& size, unsigned index) const;
	GLFramebuffer& _GetFramebuffer(unsigned id) const;
	unsigned _NextUnusedFramebufferIndex() const;

	const GLFramebuffer& _RenderBloom(const GLFramebuffer& input, unsigned& inputIndex) const;

	bool _has_bloom = false;
	Bloom _bloom;

	mutable GLFramebuffer _temp_buffer_1;
	mutable GLFramebuffer _temp_buffer_2;
	mutable GLFramebuffer _temp_buffer_3;
	mutable bool _temp_buffer_use[3];

private:
	static GLShaderProgram& _BloomShader();
	static GLShaderProgram& _CombineShader();
	static void _DrawScreenQuad();

	static std::unique_ptr<GLShaderProgram> _bloom_shader;
	static std::unique_ptr<GLShaderProgram> _combine_shader;
	static std::unique_ptr<GLBuffer> _screen_quad_buffer;
	static std::unique_ptr<GLVertexArray> _screen_quad;

};

}

#endif
