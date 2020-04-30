/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */

#ifndef RHEELENGINE_SCENERENDERMANAGER_H
#define RHEELENGINE_SCENERENDERMANAGER_H
#include "../_common.h"

#include "CustomShaderModelRenderer.h"
#include "SkyboxRenderer.h"
#include "OpenGL/Framebuffer.h"
#include "../Assets/Model.h"

namespace rheel {

class Light;
class Scene;
class SceneRenderer;
class ShadowMap;

class RE_API SceneRenderManager {
	RE_NO_COPY(SceneRenderManager);
	RE_NO_MOVE(SceneRenderManager);

	struct RE_API model_shaders {
		model_shaders();
		gl::Program forward_model_shader;
		gl::Program opaque_shader;
	};

	struct RE_API lighting_quad {
		lighting_quad();
		gl::VertexArray vao;
		gl::Buffer vbo;
	};

public:
	explicit SceneRenderManager(Scene* scene);

	/**
	 * Returns true only if shadows are enabled and at least one light in the
	 * scene casts shadows.
	 */
	bool ShouldDrawShadows() const;

	/**
	 * Updates this render manager from the scene.
	 */
	void Update();

	/**
	 * Returns a ModelRenderer instance to render the specified model. Multiple
	 * calls with the same model will result in the same model renderer.
	 */
	ModelRenderer& GetModelRenderer(const Model& model);

	/**
	 * Returns a CustomShaderModelRenderer instance to render the specified model
	 * using the specified shader. Multiple calls with the same model and shader
	 * pair will result in the same custom shader model renderer.
	 */
	CustomShaderModelRenderer& GetModelRendererForCustomShader(const Model& model, const Shader& shader);

	/**
	 * Creates and returns a SceneRenderer managed by this manager.
	 */
	std::unique_ptr<SceneRenderer> CreateSceneRenderer(std::string cameraName, unsigned width, unsigned height);

	/**
	 * Creates and returns a shadow map for the given light.
	 */
	std::unique_ptr<ShadowMap> CreateShadowMap(Light* light);

	/**
	 * Returns the scene of this render manager.
	 */
	Scene* GetScene();

	/**
	 * Returns the render map containing all models and their renderers in the
	 * scene managed by this render manager.
	 */
	const std::unordered_map<std::uintptr_t, ModelRenderer>& RenderMap() const;

	/**
	 * Returns the render map containing all models with custom shaders and their
	 * renderes in the scene managed by this render manager.
	 */
	const std::unordered_map<std::pair<std::uintptr_t, std::uintptr_t>, CustomShaderModelRenderer>& CustomShaderRenderMap() const;

	/**
	 * Returns the skybox renderer for the scene managed by this render manager
	 */
	const SkyboxRenderer& GetSkyboxRenderer() const;

	/**
	 * Returns references to all custom shader programs attached to custom shaded
	 * models managed by this render manager.
	 */
	std::vector<std::reference_wrapper<gl::Program>> CustomShaderPrograms();

	/**
	 * Initializes all lights in the shader program.
	 */
	void InitializeShaderLights(gl::Program& shaderProgram) const;

	gl::Program& GetForwardModelShader();
	gl::Program& GetOpaqueShader();

private:
	int ShadowLevel_();

	Scene* _scene;

	std::unordered_map<std::uintptr_t, ModelRenderer> _render_map;
	std::unordered_map<std::pair<uintptr_t, uintptr_t>, CustomShaderModelRenderer> _custom_shader_render_map;
	std::shared_ptr<SkyboxRenderer> _skybox_renderer;

	std::vector<int> _lights_type;
	std::vector<vec3> _lights_position;
	std::vector<vec3> _lights_direction;
	std::vector<vec4> _lights_color;
	std::vector<float> _lights_attenuation;
	std::vector<float> _lights_spot_attenuation;
	int _shadow_level{};

	pseudo_static_pointer<model_shaders> _model_shaders;
	pseudo_static_pointer<lighting_quad> _lighting_quad;

};

}

#endif
