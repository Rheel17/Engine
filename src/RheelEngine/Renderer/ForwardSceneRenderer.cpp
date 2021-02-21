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

ForwardSceneRenderer::ForwardSceneRenderer(SceneRenderManager* manager, ConstEntityId camera_entity, unsigned width, unsigned height, unsigned sample_count) :
		SceneRenderer(manager, camera_entity, width, height, sample_count, true) {}

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
		auto model_shaders = GetManager()->CustomShaderPrograms();
		model_shaders.emplace_back(GetManager()->GetForwardModelShader());

		// initialize the model shaders
		for (auto model_shader_ref : model_shaders) {
			gl::Program& model_shader = model_shader_ref;

			GetManager()->InitializeShaderLights(model_shader);
			model_shader["_cameraMatrix"] = camera->CreateMatrix(Width(), Height());

			if (model_shader.HasUniform("_cameraPosition")) {
				model_shader["_cameraPosition"] = camera->GetEntity().AbsoluteTransform().GetTranslation();
			}
		}

		// bind the shadow objects
		int shadow_map_count = 0;
		int texture_unit = 3;

		if (GetManager()->ShouldDrawShadows()) {
			const auto& shadow_maps = ShadowMaps();

			auto iter = std::find_if(shadow_maps.begin(), shadow_maps.end(), [](const auto& entry) {
				return (bool) dynamic_cast<ShadowMapDirectional*>(entry.second.get());
			});

			if (iter != shadow_maps.end()) {
				auto sm = dynamic_cast<ShadowMapDirectional*>(iter->second.get());
				shadow_map_count = sm->Textures().size();

				for (int i = 0; i < shadow_map_count; i++) {
					sm->Textures()[i].get().Bind(texture_unit++);

					for (auto model_shader_ref : model_shaders) {
						gl::Program& model_shader = model_shader_ref;
						std::string uniform_name = "_lightspaceMatrix" + std::to_string(i);

						if (model_shader.HasUniform(uniform_name)) {
							model_shader[uniform_name] = sm->LightMatrices()[i];
						}
					}
				}

				for (auto model_shader_ref : model_shaders) {
					gl::Program& model_shader = model_shader_ref;

					if (model_shader.HasUniform("_shadowMapCount")) {
						model_shader["_shadowMapCount"] = shadow_map_count;
					}
					if (model_shader.HasUniform("_baseBias")) {
						model_shader["_baseBias"] = sm->Bias();
					}
				}
			}
		}

		// Bind empty shadow map textures to the remaining (if any) shadow map
		// texture units, to make sure OpenGL doesn't complain about non-depth
		// textures bound to sampler2DShadow uniforms.
		for (int i = shadow_map_count; i < 4; i++) {
			_empty_shadow_map->texture.Bind(texture_unit++);
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
