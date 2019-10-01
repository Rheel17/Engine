/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "AmbientOcclusion.h"

#include "../../EngineResources.h"

namespace rheel {

std::unique_ptr<GLShaderProgram> AmbientOcclusion::_ssao_shader;

AmbientOcclusion::AmbientOcclusion() {}

const GLFramebuffer& AmbientOcclusion::Render(const GLFramebuffer& input) const {
	// prepare the input: if it is multi-sampled, blit it to an internal buffer
	bool useBlit = _PrepareInput();

	// setup the shader
	GLShaderProgram& ssaoShader = _SSAOShader();
	ssaoShader.Use();

	// setup the framebuffer
	unsigned idx = _UnusedFramebufferIndex();
	GLFramebuffer& buf = _Framebuffer(idx);
	_MarkFramebufferUse(idx, true);

	// render the effect
	input.Textures()[0].Bind(0);

	if (useBlit) {
		_blit_buffer->Textures()[0].Bind(1);
		_blit_buffer->Textures()[1].Bind(2);
	} else {
		_InputBuffer().Textures()[1].Bind(1);
		_InputBuffer().Textures()[2].Bind(2);
	}

	buf.Bind();
	_DrawScreenQuad();

	return buf;
}

bool AmbientOcclusion::_PrepareInput() const {
	if (!_InputBuffer().IsMultisampled()) {
		return false;
	}

	// create blit buffer if not yet created
	if (!_blit_buffer) {
		_blit_buffer = std::make_unique<GLFramebuffer>(_InputBuffer().Width(), _InputBuffer().Height());
		_blit_buffer->AddTexture(GL_RGB16F, GL_RGB); // position
		_blit_buffer->AddTexture(GL_RGB16F, GL_RGB); // normal
		_blit_buffer->Create();
	}

	// resize blit buffer if necessary
	if (_blit_buffer->Width() != _InputBuffer().Width() || _blit_buffer->Height() != _InputBuffer().Height()) {
		*_blit_buffer = _blit_buffer->ResizedCopy(_InputBuffer().Width(), _InputBuffer().Height());
	}

//	// blit the textures by hacking the internals
//	// position texture
//	_InputBuffer().Bind();
//	glReadBuffer(GL_COLOR_ATTACHMENT1);
//
//	_blit_buffer->Bind();
//	glDrawBuffer(GL_COLOR_ATTACHMENT0);
//
//	_InputBuffer().BindForReading();
//	_blit_buffer->BindForDrawing();
//
//	glBlitFramebuffer(
//			0, 0, _InputBuffer().Width(), _InputBuffer().Height(),
//			0, 0, _blit_buffer->Width(), _blit_buffer->Height(),
//			GL_COLOR_BUFFER_BIT, GL_NEAREST);
//
//	// normal texture
//	_InputBuffer().Bind();
//	glReadBuffer(GL_COLOR_ATTACHMENT2);
//
//	_blit_buffer->Bind();
//	glDrawBuffer(GL_COLOR_ATTACHMENT1);
//
//	_InputBuffer().BindForReading();
//	_blit_buffer->BindForDrawing();
//
//	glBlitFramebuffer(
//			0, 0, _InputBuffer().Width(), _InputBuffer().Height(),
//			0, 0, _blit_buffer->Width(), _blit_buffer->Height(),
//			GL_COLOR_BUFFER_BIT, GL_NEAREST);
//
//	// reset
//	_InputBuffer().Bind();
//	glReadBuffer(GL_COLOR_ATTACHMENT0);

	return true;
}

GLShaderProgram& AmbientOcclusion::_SSAOShader() {
	if (!_ssao_shader) {
		_ssao_shader = std::make_unique<GLShaderProgram>();
		_ssao_shader->AddShaderFromSource(GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_postprocessing_vert_glsl"));
		_ssao_shader->AddShaderFromSource(GLShaderProgram::FRAGMENT, EngineResources::PreprocessShader("Shaders_postprocessing_ssao_frag_glsl"));
		_ssao_shader->Link();
		_ssao_shader->GetUniform("colorTexture") = 0;
		_ssao_shader->GetUniform("positionTexture") = 1;
		_ssao_shader->GetUniform("normalTexture") = 2;
	}

	return *_ssao_shader;
}

}
