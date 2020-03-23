/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "SceneRenderManager.h"

#include "ForwardSceneRenderer.h"
#include "ShadowMapDirectional.h"
#include "SkyboxRenderer.h"
#include "../Engine.h"
#include "../EngineResources.h"
#include "../Components/DirectionalLight.h"
#include "../Components/PointLight.h"
#include "../Components/SpotLight.h"

namespace rheel {

std::unique_ptr<_GLVertexArray> SceneRenderManager::_lighting_quad_vao(nullptr);
std::unique_ptr<_GLBuffer> SceneRenderManager::_lighting_quad_vbo(nullptr);
bool SceneRenderManager::_lighting_quad_initialized = false;

SceneRenderManager::SceneRenderManager(Scene *scene) :
		_scene(scene), _skybox_renderer(std::make_shared<SkyboxRenderer>(this)) {

	_Initialize();
}

bool SceneRenderManager::ShouldDrawShadows() const {
	return _shadow_level > 0;
}

void SceneRenderManager::Update() {
	_lights_type.clear();
	_lights_position.clear();
	_lights_direction.clear();
	_lights_color.clear();
	_lights_attenuation.clear();
	_lights_spot_attenuation.clear();

	for (Light *light : _scene->GetLights()) {
		_lights_color.push_back(light->GetColor());

		if (auto pointLight = dynamic_cast<PointLight *>(light)) {
			_lights_type.push_back(0);
			_lights_position.push_back(pointLight->Position());
			_lights_direction.emplace_back();
			_lights_attenuation.push_back(pointLight->Attenuation());
			_lights_spot_attenuation.push_back(0.0f);
		} else if (auto spotLight = dynamic_cast<SpotLight *>(light)) {
			_lights_type.push_back(1);
			_lights_position.push_back(spotLight->Position());
			_lights_direction.push_back(spotLight->Direction());
			_lights_attenuation.push_back(spotLight->Attenuation());
			_lights_spot_attenuation.push_back(spotLight->SpotAttenuation());
		} else if (auto directionalLight = dynamic_cast<DirectionalLight *>(light)) {
			_lights_type.push_back(2);
			_lights_position.emplace_back();
			_lights_direction.push_back(directionalLight->Direction());
			_lights_attenuation.push_back(0.0f);
			_lights_spot_attenuation.push_back(0.0f);
		}
	}

	_shadow_level = _ShadowLevel();
}

ModelRenderer& SceneRenderManager::GetModelRenderer(const Model& model) {
	auto iter = _render_map.find(model.GetAddress());

	if (iter == _render_map.end()) {
		iter = _render_map.emplace(model.GetAddress(), model).first;
	}

	return iter->second;
}

CustomShaderModelRenderer& SceneRenderManager::GetModelRendererForCustomShader(const Model& model, const Shader& shader) {
	auto iter = _custom_shader_render_map.find(std::make_pair(model.GetAddress(), shader.GetAddress()));

	if (iter == _custom_shader_render_map.end()) {
		iter = _custom_shader_render_map.emplace(std::make_pair(model.GetAddress(), shader.GetAddress()), CustomShaderModelRenderer(model, shader)).first;
	}

	return iter->second;
}

std::unique_ptr<SceneRenderer> SceneRenderManager::CreateSceneRenderer(std::string cameraName, unsigned width, unsigned height) {
	return std::unique_ptr<ForwardSceneRenderer>(new ForwardSceneRenderer(this, std::move(cameraName), width, height));
}

std::unique_ptr<ShadowMap> SceneRenderManager::CreateShadowMap(Light *light) {
	if (dynamic_cast<PointLight *>(light)) {

	} else if (dynamic_cast<SpotLight *>(light)) {

	} else if (dynamic_cast<DirectionalLight *>(light)) {
		return std::unique_ptr<ShadowMapDirectional>(new ShadowMapDirectional(this, light));
	}

	return nullptr;
}

Scene *SceneRenderManager::GetScene() {
	return _scene;
}

const std::unordered_map<std::uintptr_t, ModelRenderer>& SceneRenderManager::RenderMap() const {
	return _render_map;
}

const std::unordered_map<std::pair<std::uintptr_t, std::uintptr_t>, CustomShaderModelRenderer>& SceneRenderManager::CustomShaderRenderMap() const {
	return _custom_shader_render_map;
}

const SkyboxRenderer& SceneRenderManager::GetSkyboxRenderer() const {
	return *_skybox_renderer;
}

std::vector<std::reference_wrapper<_GLShaderProgram>> SceneRenderManager::CustomShaderPrograms() {
	std::vector<std::reference_wrapper<_GLShaderProgram>> shaders;

	for (auto& [key, value] : _custom_shader_render_map) {
		shaders.emplace_back(value.GetShaderProgram());
	}

	return shaders;
}

void SceneRenderManager::InitializeShaderLights(_GLShaderProgram& shaderProgram) const {
	if (shaderProgram.HasUniform("_lights_type")) shaderProgram["_lights_type"] = _lights_type;
	if (shaderProgram.HasUniform("_lights_position")) shaderProgram["_lights_position"] = _lights_position;
	if (shaderProgram.HasUniform("_lights_direction")) shaderProgram["_lights_direction"] = _lights_direction;
	if (shaderProgram.HasUniform("_lights_color")) shaderProgram["_lights_color"] = _lights_color;
	if (shaderProgram.HasUniform("_lights_attenuation")) shaderProgram["_lights_attenuation"] = _lights_attenuation;
	if (shaderProgram.HasUniform("_lights_spot_attenuation")) shaderProgram["_lights_spot_attenuation"] = _lights_spot_attenuation;
	if (shaderProgram.HasUniform("_lightCount")) shaderProgram["_lightCount"] = (GLint) _lights_type.size();
	if (shaderProgram.HasUniform("_shadowLevel")) shaderProgram["_shadowLevel"] = (GLint) _shadow_level;
}

int SceneRenderManager::_ShadowLevel() {
	for (Light *light : _scene->GetLights()) {
		if (light->CastsShadows()) {
			return Engine::GetDisplayConfiguration().shadow_quality;
		}
	}

	return 0;
}

void SceneRenderManager::_Initialize() {
	if (_lighting_quad_initialized) {
		return;
	}

	GLfloat triangles[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
	_lighting_quad_vbo = std::make_unique<_GLBuffer>(_GL::BufferTarget::ARRAY);
	_lighting_quad_vbo->SetData(triangles, sizeof(triangles));

	_lighting_quad_vao = std::make_unique<_GLVertexArray>();
	_lighting_quad_vao->SetVertexAttributes<vec2>(*_lighting_quad_vbo);

	_lighting_quad_initialized = true;
}

}
