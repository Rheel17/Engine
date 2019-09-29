/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef POSTPROCESSINGSTACK_H_
#define POSTPROCESSINGSTACK_H_
#include "../_common.h"

#include "OpenGL/GLFramebuffer.h"
#include "OpenGL/GLShaderProgram.h"

namespace rheel {

class RE_API PostProcessingStack {

public:
	struct Bloom {
		float threshold_start;
		float threshold_end;
		float multiplier;
		unsigned level;
	};

public:
	PostProcessingStack();

	void Render(const GLFramebuffer& input, const ivec2& pos, const ivec2& size) const;

	void SetBloom(float thresholdStart, float thresholdEnd, float multiplier, unsigned level);
	void SetBloomOff();
	const Bloom& GetBloom() const;

private:
	void _EnsureTempBuffersSize(const ivec2& size, unsigned index) const;

	const GLFramebuffer& _RenderBloom(const GLFramebuffer& input) const;

	bool _has_bloom = false;
	Bloom _bloom;

	mutable GLFramebuffer _temp_buffer_1;
	mutable GLFramebuffer _temp_buffer_2;
	mutable unsigned _temp_buffer_use = 0;

private:
	static GLShaderProgram& _BloomShader();

	static std::unique_ptr<GLShaderProgram> _bloom_shader;

};

}

#endif
