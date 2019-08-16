#include "SceneRenderManager.h"

#include "ModelRenderer.h"
#include "../Engine.h"
#include "../Resources.h"

#include <unordered_set>

namespace rheel {

GLShaderProgram SceneRenderManager::_lighting_shader;
std::shared_ptr<GLVertexArray> SceneRenderManager::_lighting_quad_vao(nullptr);
std::shared_ptr<GLBuffer> SceneRenderManager::_lighting_quad_vbo(nullptr);
bool SceneRenderManager::_lighting_quad_initialized = false;

SceneRenderManager::SceneRenderManager(Scene_t *scene) :
		_scene(scene) {

	_Initialize();
}

/**
 * Returns true only if shadows are enabled and at least one light in the
 * scene casts shadows.
 */
bool SceneRenderManager::ShouldDrawShadows() const {
	if (Engine::GetDisplayConfiguration().shadow_quality == DisplayConfiguration::SHADOW_OFF) {
		return false;
	}

	for (const std::string& light : _scene->Lights()) {
		if (_scene->GetLight(light)->CastsShadows()) {
			return true;
		}
	}

	return false;
}

void SceneRenderManager::Update() {
	_lights_type.clear();
	_lights_position.clear();
	_lights_direction.clear();
	_lights_color.clear();
	_lights_attenuation.clear();
	_lights_spot_attenuation.clear();

	for (const std::string& lightName : _scene->Lights()) {
		Light *light = _scene->GetLight(lightName);

		_lights_type.push_back(light->Type());
		_lights_position.push_back(light->Position());
		_lights_direction.push_back(light->Direction());
		_lights_color.push_back(light->Color());
		_lights_attenuation.push_back(light->Attenuation());
		_lights_spot_attenuation.push_back(light->SpotAttenuation());
	}
}

ModelRenderer& SceneRenderManager::GetModelRenderer(ModelPtr model) {
	auto iter = _render_map.find(model);

	if (iter == _render_map.end()) {
		iter = _render_map.emplace(model, model).first;
	}

	return iter->second;
}

SceneRenderer SceneRenderManager::CreateSceneRenderer(std::string cameraName, unsigned width, unsigned height) {
	return SceneRenderer(this, std::move(cameraName), width, height);
}

SceneRenderManager::Scene_t *SceneRenderManager::Scene() {
	return _scene;
}

const std::unordered_map<ModelPtr, ModelRenderer>& SceneRenderManager::RenderMap() const {
	return _render_map;
}

GLShaderProgram& SceneRenderManager::InitializedLightingShader() const {
	_lighting_shader["lights_type"] = _lights_type;
	_lighting_shader["lights_position"] = _lights_position;
	_lighting_shader["lights_direction"] = _lights_direction;
	_lighting_shader["lights_color"] = _lights_color;
	_lighting_shader["lights_attenuation"] = _lights_attenuation;
	_lighting_shader["lights_spot_attenuation"] = _lights_spot_attenuation;
	_lighting_shader["lightCount"] = (GLint) _lights_type.size();
	return _lighting_shader;
}

void SceneRenderManager::DrawLightingQuad() const {
	_lighting_quad_vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneRenderManager::_Initialize() {
	if (_lighting_quad_initialized) {
		return;
	}

	_lighting_shader.AddShaderFromSource(GLShaderProgram::VERTEX, RESOURCE_AS_STRING(Shaders_lightingshader_vert_glsl));
	_lighting_shader.AddShaderFromSource(GLShaderProgram::FRAGMENT, RESOURCE_AS_STRING(Shaders_lightingshader_frag_glsl));
	_lighting_shader.Link();
	_lighting_shader["gBufferColor"] = 0;
	_lighting_shader["gBufferPosition"] = 1;
	_lighting_shader["gBufferNormal"] = 2;
	_lighting_shader["gBufferAmbient"] = 3;
	_lighting_shader["gBufferDiffuse"] = 4;
	_lighting_shader["gBufferSpecular"] = 5;
	_lighting_shader["gBufferMaterialParameters"] = 6;

	GLfloat triangles[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
	_lighting_quad_vbo = std::make_shared<GLBuffer>(GL::BufferTarget::ARRAY);
	_lighting_quad_vbo->SetData(triangles, sizeof(triangles));

	_lighting_quad_vao = std::make_shared<GLVertexArray>();
	_lighting_quad_vao->SetVertexAttributes<vec2>(*_lighting_quad_vbo);

	_lighting_quad_initialized = true;
}

}
