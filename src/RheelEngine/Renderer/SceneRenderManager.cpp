/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "SceneRenderManager.h"

#include "ForwardSceneRenderer.h"
#include "ShadowMapDirectional.h"
#include "../EngineResources.h"
#include "../Components/DirectionalLight.h"
#include "../Components/PointLight.h"
#include "../Components/SpotLight.h"

namespace rheel {

SceneRenderManager::model_shaders::model_shaders() {
	gl::ContextScope cs;

	forward_model_shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_modelshader_vert_glsl"));
	forward_model_shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_modelshader_frag_glsl"));
	forward_model_shader.Link();
	forward_model_shader["ambientTexture"] = 0;
	forward_model_shader["diffuseTexture"] = 1;
	forward_model_shader["specularTexture"] = 2;
	forward_model_shader["_shadowMap0"] = 3;
	forward_model_shader["_shadowMap1"] = 4;
	forward_model_shader["_shadowMap2"] = 5;
	forward_model_shader["_shadowMap3"] = 6;

	opaque_shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_opaqueshader_vert_glsl"));
	opaque_shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_opaqueshader_frag_glsl"));
	opaque_shader.Link();
}

SceneRenderManager::SceneRenderManager(Scene* scene) :
		_scene(scene),
		_skybox_renderer(std::make_shared<SkyboxRenderer>(this)) {}

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

	for (Light* light : _scene->GetLights()) {
		_lights_color.push_back(light->GetColor());

		if (auto pointLight = dynamic_cast<PointLight*>(light)) {
			_lights_type.push_back(0);
			_lights_position.push_back(pointLight->Position());
			_lights_direction.emplace_back();
			_lights_attenuation.push_back(pointLight->Attenuation());
			_lights_spot_attenuation.push_back(0.0f);
		} else if (auto spotLight = dynamic_cast<SpotLight*>(light)) {
			_lights_type.push_back(1);
			_lights_position.push_back(spotLight->Position());
			_lights_direction.push_back(spotLight->Direction());
			_lights_attenuation.push_back(spotLight->Attenuation());
			_lights_spot_attenuation.push_back(spotLight->SpotAttenuation());
		} else if (auto directionalLight = dynamic_cast<DirectionalLight*>(light)) {
			_lights_type.push_back(2);
			_lights_position.emplace_back();
			_lights_direction.push_back(directionalLight->Direction());
			_lights_attenuation.push_back(0.0f);
			_lights_spot_attenuation.push_back(0.0f);
		}
	}

	_shadow_level = ShadowLevel_();
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
	return std::unique_ptr<ForwardSceneRenderer>(
			new ForwardSceneRenderer(this, std::move(cameraName), width, height, DisplayConfiguration::Get().SampleCount()));
}

std::unique_ptr<ShadowMap> SceneRenderManager::CreateShadowMap(Light* light) {
	if (dynamic_cast<PointLight*>(light)) {

	} else if (dynamic_cast<SpotLight*>(light)) {

	} else if (dynamic_cast<DirectionalLight*>(light)) {
		return std::unique_ptr<ShadowMapDirectional>(new ShadowMapDirectional(this, light));
	}

	return nullptr;
}

Scene* SceneRenderManager::GetScene() {
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

std::vector<std::reference_wrapper<gl::Program>> SceneRenderManager::CustomShaderPrograms() {
	std::vector<std::reference_wrapper<gl::Program>> shaders;

	for (auto&[key, value] : _custom_shader_render_map) {
		shaders.emplace_back(value.GetShaderProgram());
	}

	return shaders;
}

void SceneRenderManager::InitializeShaderLights(gl::Program& shaderProgram) const {
	if (shaderProgram.HasUniform("_lights_type")) {
		shaderProgram["_lights_type"] = _lights_type;
	}

	if (shaderProgram.HasUniform("_lights_position")) {
		shaderProgram["_lights_position"] = _lights_position;
	}

	if (shaderProgram.HasUniform("_lights_direction")) {
		shaderProgram["_lights_direction"] = _lights_direction;
	}

	if (shaderProgram.HasUniform("_lights_color")) {
		shaderProgram["_lights_color"] = _lights_color;
	}

	if (shaderProgram.HasUniform("_lights_attenuation")) {
		shaderProgram["_lights_attenuation"] = _lights_attenuation;
	}

	if (shaderProgram.HasUniform("_lights_spot_attenuation")) {
		shaderProgram["_lights_spot_attenuation"] = _lights_spot_attenuation;
	}

	if (shaderProgram.HasUniform("_lightCount")) {
		shaderProgram["_lightCount"] = (GLint) _lights_type.size();
	}

	if (shaderProgram.HasUniform("_shadowLevel")) {
		shaderProgram["_shadowLevel"] = (GLint) _shadow_level;
	}
}

gl::Program& SceneRenderManager::GetForwardModelShader() {
	return _model_shaders->forward_model_shader;
}

gl::Program& SceneRenderManager::GetOpaqueShader() {
	return _model_shaders->opaque_shader;
}

int SceneRenderManager::ShadowLevel_() {
	for (Light* light : _scene->GetLights()) {
		if (light->CastsShadows()) {
			return DisplayConfiguration::Get().shadow_quality;
		}
	}

	return 0;
}

}
