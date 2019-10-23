/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "ForwardSceneRenderer.h"

#include "ShadowMapDirectional.h"
#include "../Engine.h"
#include "../Scene.h"

namespace rheel {

ForwardSceneRenderer::ForwardSceneRenderer(SceneRenderManager *manager, std::string cameraName, unsigned width, unsigned height) :
		SceneRenderer(manager, cameraName, width, height,
				Engine::GetDisplayConfiguration().SampleCount(), true) {}

void ForwardSceneRenderer::Render(float dt) {
	// get the camera
	Camera *camera = GetCamera();

	// if no camera with the given name was found: don't render anything
	// new to the buffer.
	if (!camera) {
		return;
	}

	// render the shadows
	_RenderShadowMaps();

	// bind and reset the result buffer to start writing there
	ResultBuffer().Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// initialize the model shader
	GLShaderProgram& modelShader = ModelRenderer::GetForwardModelShader();
	GetManager()->InitializeShaderLights(modelShader);
	modelShader["cameraMatrix"] = camera->CreateMatrix(Width(), Height());
	modelShader["cameraPosition"] = camera->transform.GetTranslation();

	// bind the shadow objects
	int shadowMapCount = 0;
	int textureUnit = 3;

	if (GetManager()->ShouldDrawShadows()) {
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

	// render all the models
	for (const auto& pair : GetManager()->RenderMap()) {
		pair.second.RenderObjects();
	}

	// clear everything again to return to normal
	GL::ClearFramebufferBinding();
	GLShaderProgram::ClearUse();
}

void ForwardSceneRenderer::Resize(unsigned width, unsigned height) {}

}
