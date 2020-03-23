/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ForwardSceneRenderer.h"

#include <utility>

#include "ShadowMapDirectional.h"
#include "../Engine.h"
#include "../Scene.h"

namespace rheel {

ForwardSceneRenderer::ForwardSceneRenderer(SceneRenderManager *manager, std::string cameraName, unsigned width, unsigned height) :
		SceneRenderer(manager, std::move(cameraName), width, height,
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

	// render the skybox
	_RenderSkybox(Width(), Height());

	// get the shaders for the custom shaded models, add the normal
	// model shader
	auto modelShaders = GetManager()->CustomShaderPrograms();
	modelShaders.emplace_back(ModelRenderer::GetForwardModelShader());

	// initialize the model shaders
	for (auto modelShaderRef : modelShaders) {
		_GLShaderProgram& modelShader = modelShaderRef;

		GetManager()->InitializeShaderLights(modelShader);
		modelShader["_cameraMatrix"] = camera->CreateMatrix(Width(), Height());

		if (modelShader.HasUniform("_cameraPosition")) {
			modelShader["_cameraPosition"] = camera->CalculateAbsoluteTransform().GetTranslation();
		}
	}

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

				for (auto modelShaderRef : modelShaders) {
					_GLShaderProgram& modelShader = modelShaderRef;
					std::string uniformName = "_lightspaceMatrix" + std::to_string(i);

					if (modelShader.HasUniform(uniformName)) {
						modelShader[uniformName] = sm->LightMatrices()[i];
					}
				}
			}

			for (auto modelShaderRef : modelShaders) {
				_GLShaderProgram& modelShader = modelShaderRef;

				if (modelShader.HasUniform("_shadowMapCount")) modelShader["_shadowMapCount"] = shadowMapCount;
				if (modelShader.HasUniform("_baseBias")) modelShader["_baseBias"] = sm->Bias();
			}
		}
	}

	// Bind empty shadow map textures to the remaining (if any) shadow map
	// texture units, to make sure _OpenGL doesn't complain about non-depth
	// textures bound to sampler2DShadow uniforms.
	for (int i = shadowMapCount; i < 4; i++) {
		ShadowMapDirectional::EmptyShadowMap().Bind(textureUnit++);
	}

	// render all objects
	for (const auto entity : GetManager()->GetScene()->GetEntities()) {
		entity->Render();
	}

	// render all the models
	for (const auto& [_, renderer] : GetManager()->RenderMap()) {
		renderer.RenderObjects();
	}

	// render all models with custom shaders
	for (const auto& [_, renderer] : GetManager()->CustomShaderRenderMap()) {
		renderer.RenderObjects();
	}

	// clear everything again to return to normal
	_GL::ClearFramebufferBinding();
	_GLShaderProgram::ClearUse();
}

void ForwardSceneRenderer::Resize(unsigned width, unsigned height) {}

}
