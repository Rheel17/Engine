/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef AMBIENTOCCLUSION_H_
#define AMBIENTOCCLUSION_H_
#include "../../_common.h"

#include "../PostProcessingEffect.h"
#include "../OpenGL/GLShaderProgram.h"

namespace rheel {

class RE_API AmbientOcclusion : public PostProcessingEffect {

public:
	AmbientOcclusion();

	const GLFramebuffer& Render(const GLFramebuffer& input) const override;

private:
	bool _PrepareInput() const;

	mutable std::unique_ptr<GLFramebuffer> _blit_buffer;

private:
	static GLShaderProgram& _SSAOShader();

	static std::unique_ptr<GLShaderProgram> _ssao_shader;

};

}

#endif
