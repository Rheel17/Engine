#ifndef SCENERENDERMANAGER_H_
#define SCENERENDERMANAGER_H_
#include "../_common.h"

#include "../Scene.h"
#include "OpenGL/GLFramebuffer.h"
#include "ModelRenderer.h"
#include "SceneRenderer.h"

namespace rheel {

class RE_API SceneRenderManager {
	using Scene_t = rheel::Scene;

public:
	SceneRenderManager(Scene_t *scene);

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
	 * Returns a ModelRenderer instance to render the specified model.
	 * Multiple calls with the same model will result in the same
	 * model renderer.
	 */
	ModelRenderer& GetModelRenderer(ModelPtr model);

	/**
	 * Creates and returns a SceneRenderer managed by this manager.
	 */
	SceneRenderer CreateSceneRenderer(std::string cameraName, unsigned width, unsigned height);

	/**
	 * Returns the scene of this render manager.
	 */
	Scene_t *Scene();

	/**
	 * Returns the render map containing all models and their renderers
	 * in the scene managed by this render manager.
	 */
	const std::unordered_map<ModelPtr, ModelRenderer>& RenderMap() const;

	/**
	 * Returns a reference to the lighting shader, with all lights
	 * initialized.
	 */
	GLShaderProgram& InitializedLightingShader() const;

	/**
	 * Binds and draws the VAO of the lighting quad.
	 */
	void DrawLightingQuad() const;

private:
	Scene_t *_scene;
	std::unordered_map<ModelPtr, ModelRenderer> _render_map;

	std::vector<int> _lights_type;
	std::vector<vec3> _lights_position;
	std::vector<vec3> _lights_direction;
	std::vector<vec4> _lights_color;
	std::vector<float> _lights_attenuation;
	std::vector<float> _lights_spot_attenuation;

private:
	static void _Initialize();

	static GLShaderProgram _lighting_shader;
	static std::shared_ptr<GLVertexArray> _lighting_quad_vao;
	static std::shared_ptr<GLBuffer> _lighting_quad_vbo;
	static bool _lighting_quad_initialized;

};

}

#endif
