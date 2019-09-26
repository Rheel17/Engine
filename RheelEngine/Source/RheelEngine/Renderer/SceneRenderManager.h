/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef SCENERENDERMANAGER_H_
#define SCENERENDERMANAGER_H_
#include "../_common.h"

#include "ModelRenderer.h"
#include "../ModelResource.h"
#include "../Scene.h"
#include "OpenGL/GLFramebuffer.h"

namespace rheel {

class SceneRenderer;
class ShadowMap;

class RE_API SceneRenderManager {

public:
	SceneRenderManager(Scene *scene);

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
	ModelRenderer& GetModelRenderer(ModelResource& model);

	/**
	 * Creates and returns a SceneRenderer managed by this manager.
	 */
	std::unique_ptr<SceneRenderer> CreateSceneRenderer(std::string cameraName, unsigned width, unsigned height);

	/**
	 * Creates and returns a shadow map for the given light.
	 */
	std::unique_ptr<ShadowMap> CreateShadowMap(const std::string& lightName);

	/**
	 * Returns the scene of this render manager.
	 */
	Scene *GetScene();

	/**
	 * Returns the render map containing all models and their renderers in the
	 * scene managed by this render manager.
	 */
	const std::unordered_map<Model *, ModelRenderer>& RenderMap() const;

	/**
	 * Returns a reference to the lighting shader, with all lights initialized.
	 */
	GLShaderProgram& InitializedDeferredLightingShader() const;

	/**
	 * Initializes all lights in the shader program.
	 */
	void InitializeShaderLights(GLShaderProgram& shaderProgram) const;

	/**
	 * Binds and draws the VAO of the lighting quad.
	 */
	void DrawDeferredLightingQuad() const;

private:
	int _ShadowLevel();

	Scene *_scene;
	std::unordered_map<Model *, ModelRenderer> _render_map;

	std::vector<int> _lights_type;
	std::vector<vec3> _lights_position;
	std::vector<vec3> _lights_direction;
	std::vector<vec4> _lights_color;
	std::vector<float> _lights_attenuation;
	std::vector<float> _lights_spot_attenuation;
	int _shadow_level;

private:
	static void _Initialize();

	static GLShaderProgram _deferred_lighting_shader;
	static std::unique_ptr<GLVertexArray> _lighting_quad_vao;
	static std::unique_ptr<GLBuffer> _lighting_quad_vbo;
	static bool _lighting_quad_initialized;

};

}

#endif
