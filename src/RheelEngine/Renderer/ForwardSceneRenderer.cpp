/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ForwardSceneRenderer.h"

#include "ShadowMapDirectional.h"

namespace rheel {

ForwardSceneRenderer::empty_shadow_map::empty_shadow_map() {
	gl::ContextScope cs;

	texture.SetEmpty(gl::InternalFormat::DEPTH_COMPONENT_32F, 1, 1, gl::Format::DEPTH_COMPONENT);
	texture.SetCompareMode(gl::Texture::CompareMode::COMPARE_REF_TO_TEXTURE);
	texture.SetCompareFunction(gl::CompareFunction::LEQUAL);
}

ForwardSceneRenderer::ForwardSceneRenderer(SceneRenderManager* manager, ConstEntityId camera_entity, unsigned width, unsigned height, unsigned sampleCount) :
		SceneRenderer(manager, camera_entity, width, height, sampleCount, true) {}

void ForwardSceneRenderer::Render(float dt) {
	// get the camera
	const Camera* camera = GetCamera();

	// if no camera with the given name was found: don't render anything
	// new to the buffer.
	if (!camera) {
		return;
	}

	{
		gl::ContextScope cs;

		// render the shadows
		RenderShadowMaps();

		// bind and reset the result buffer to start writing there
		ResultBuffer().Clear(gl::Framebuffer::BitField::COLOR_DEPTH);

		// render the skybox
		RenderSkybox(Width(), Height());

		// get the shaders for the custom shaded models, add the normal
		// model shader
		auto modelShaders = GetManager()->CustomShaderPrograms();
		modelShaders.emplace_back(GetManager()->GetForwardModelShader());

		// initialize the model shaders
		for (auto modelShaderRef : modelShaders) {
			gl::Program& modelShader = modelShaderRef;

			GetManager()->InitializeShaderLights(modelShader);
			modelShader["_cameraMatrix"] = camera->CreateMatrix(Width(), Height());

			if (modelShader.HasUniform("_cameraPosition")) {
				modelShader["_cameraPosition"] = camera->GetEntity().AbsoluteTransform().GetTranslation();
			}
		}

		// bind the shadow objects
		int shadowMapCount = 0;
		int textureUnit = 3;

		if (GetManager()->ShouldDrawShadows()) {
			const auto& shadowMaps = ShadowMaps();

			auto iter = std::find_if(shadowMaps.begin(), shadowMaps.end(), [](const auto& entry) {
				return (bool) dynamic_cast<ShadowMapDirectional*>(entry.second.get());
			});

			if (iter != shadowMaps.end()) {
				auto sm = dynamic_cast<ShadowMapDirectional*>(iter->second.get());
				shadowMapCount = sm->Textures().size();

				for (int i = 0; i < shadowMapCount; i++) {
					sm->Textures()[i].get().Bind(textureUnit++);

					for (auto modelShaderRef : modelShaders) {
						gl::Program& modelShader = modelShaderRef;
						std::string uniformName = "_lightspaceMatrix" + std::to_string(i);

						if (modelShader.HasUniform(uniformName)) {
							modelShader[uniformName] = sm->LightMatrices()[i];
						}
					}
				}

				for (auto modelShaderRef : modelShaders) {
					gl::Program& modelShader = modelShaderRef;

					if (modelShader.HasUniform("_shadowMapCount")) {
						modelShader["_shadowMapCount"] = shadowMapCount;
					}
					if (modelShader.HasUniform("_baseBias")) {
						modelShader["_baseBias"] = sm->Bias();
					}
				}
			}
		}

		// Bind empty shadow map textures to the remaining (if any) shadow map
		// texture units, to make sure OpenGL doesn't complain about non-depth
		// textures bound to sampler2DShadow uniforms.
		for (int i = shadowMapCount; i < 4; i++) {
			_empty_shadow_map->texture.Bind(textureUnit++);
		}

		// render all the models
		for (const auto&[_, renderer] : GetManager()->RenderMap()) {
			renderer.RenderObjects();
		}

		// render all models with custom shaders
		for (const auto&[_, renderer] : GetManager()->CustomShaderRenderMap()) {
			renderer.RenderObjects();
		}
	}
}

void ForwardSceneRenderer::Resize(unsigned width, unsigned height) {}

}
