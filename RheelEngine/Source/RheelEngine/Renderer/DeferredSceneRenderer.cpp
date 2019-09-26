/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "DeferredSceneRenderer.h"

#include "ShadowMapDirectional.h"
#include "../Engine.h"

namespace rheel {

DeferredSceneRenderer::DeferredSceneRenderer(SceneRenderManager *manager, std::string cameraName,
		unsigned width, unsigned height) :
		SceneRenderer(manager, cameraName, width, height, 1, false),
		_g_buffer(width, height, Engine::GetDisplayConfiguration().SampleCount(), true) {

	_g_buffer.AddTexture(GL_RGBA32F, GL_RGBA); // color
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB ); // position
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB ); // normal
	_g_buffer.AddTexture(GL_RGB8,    GL_RGB ); // material:ambient
	_g_buffer.AddTexture(GL_RGB8,    GL_RGB ); // material:diffuse
	_g_buffer.AddTexture(GL_RGB8,    GL_RGB ); // material:specular
	_g_buffer.AddTexture(GL_RGBA32F, GL_RGBA); // material:parameters
	_g_buffer.AddRenderbuffer(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	_g_buffer.Create();
}

void DeferredSceneRenderer::Render(float dt) {
	// get the camera
	Camera *camera = GetCamera();

	// if no camera with the given name was found: don't render anything
	// new to the buffer.
	if (!camera) {
		return;
	}

	_RenderShadowMaps();

	// send the camera matrix to the model shader
	mat4 cameraMatrix = camera->CreateMatrix(Width(), Height());
	GLShaderProgram& modelShader = ModelRenderer::GetDeferredModelShader();
	modelShader["cameraMatrix"] = cameraMatrix;

	// write the scene to the g-buffer.
	_g_buffer.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& pair : Manager()->RenderMap()) {
		pair.second.RenderObjects();
	}

	// write the g-buffer to the result buffer with the lighting shader
	ResultBuffer().Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int textureUnit = 0;
	for (const auto& texture : _g_buffer.MultisampleTextures()) {
		texture.Bind(textureUnit++);
	}

	GLShaderProgram& shader = Manager()->InitializedDeferredLightingShader();
	shader["gBufferTextureSize"] = ivec2 { Width(), Height() };
	shader["sampleCount"] = (GLint) Engine::GetDisplayConfiguration().SampleCount();
	shader["cameraPosition"] = camera->Position();

	// bind the shadow objects
	int shadowMapCount = 0;

	if (Manager()->ShouldDrawShadows()) {
		const auto& shadowMaps = ShadowMaps();

		auto iter = std::find_if(shadowMaps.begin(), shadowMaps.end(), [](const auto& entry) {
			return (bool) dynamic_cast<ShadowMapDirectional *>(entry.second.get());
		});

		if (iter != shadowMaps.end()) {
			auto sm = dynamic_cast<ShadowMapDirectional *>(iter->second.get());
			shadowMapCount = sm->Textures().size();

			for (int i = 0; i < shadowMapCount; i++) {
				sm->Textures()[i].Bind(textureUnit++);
				modelShader["lightspaceMatrix" + std::to_string(i)] = sm->LightMatrices()[i];
			}

			modelShader["shadowMapCount"] = shadowMapCount;
			modelShader["baseBias"] = sm->Bias();
		}
	}

	// Bind empty shadow map textures to the remaining (if any) shadow map
	// texture units, to make sure OpenGL doesn't complain about non-depth
	// textures bound to sampler2DShadow uniforms.
	for (int i = shadowMapCount; i < 4; i++) {
		ShadowMapDirectional::EmptyShadowMap().Bind(textureUnit++);
	}

	Manager()->DrawDeferredLightingQuad();

	GL::ClearFramebufferBinding();
	GLShaderProgram::ClearUse();

	while (--textureUnit >= 0) {
		GL::ClearTextureBinding(GL::TextureTarget::TEXTURE_2D, textureUnit);
	}
}

void DeferredSceneRenderer::Resize(unsigned width, unsigned height) {
	_g_buffer = _g_buffer.ResizedCopy(width, height);
}

}
