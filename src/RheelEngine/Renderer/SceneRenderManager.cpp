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

#include <algorithm>

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

	for (const auto& point_light : _scene->GetRegistry().GetComponents<PointLight>()) {
		_lights_type.push_back(0);
		_lights_position.push_back(point_light.Position());
		_lights_direction.emplace_back();
		_lights_color.push_back(point_light.GetColor());
		_lights_attenuation.push_back(point_light.Attenuation());
		_lights_spot_attenuation.push_back(0.0f);
	}

	for (const auto& spot_light : _scene->GetRegistry().GetComponents<SpotLight>()) {
		_lights_type.push_back(1);
		_lights_position.push_back(spot_light.Position());
		_lights_direction.push_back(spot_light.Direction());
		_lights_color.push_back(spot_light.GetColor());
		_lights_attenuation.push_back(spot_light.Attenuation());
		_lights_spot_attenuation.push_back(spot_light.SpotAttenuation());
	}

	for (const auto& directional_light : _scene->GetRegistry().GetComponents<DirectionalLight>()) {
		_lights_type.push_back(2);
		_lights_position.emplace_back();
		_lights_direction.push_back(directional_light.Direction());
		_lights_color.push_back(directional_light.GetColor());
		_lights_attenuation.push_back(0.0f);
		_lights_spot_attenuation.push_back(0.0f);
	}

	_shadow_level = _get_shadow_quality();
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

std::unique_ptr<SceneRenderer> SceneRenderManager::CreateSceneRenderer(ConstEntityId camera_entity, unsigned width, unsigned height) {
	return std::unique_ptr<ForwardSceneRenderer>(
			new ForwardSceneRenderer(this, camera_entity, width, height, DisplayConfiguration::Get().SampleCount()));
}

std::unique_ptr<ShadowMap> SceneRenderManager::CreateShadowMap(const Light& light) {
	if (dynamic_cast<const PointLight*>(&light)) {

	} else if (dynamic_cast<const SpotLight*>(&light)) {

	} else if (dynamic_cast<const DirectionalLight*>(&light)) {
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

void SceneRenderManager::InitializeShaderLights(gl::Program& shader_program) const {
	if (shader_program.HasUniform("_lights_type")) {
		shader_program["_lights_type"] = _lights_type;
	}

	if (shader_program.HasUniform("_lights_position")) {
		shader_program["_lights_position"] = _lights_position;
	}

	if (shader_program.HasUniform("_lights_direction")) {
		shader_program["_lights_direction"] = _lights_direction;
	}

	if (shader_program.HasUniform("_lights_color")) {
		shader_program["_lights_color"] = _lights_color;
	}

	if (shader_program.HasUniform("_lights_attenuation")) {
		shader_program["_lights_attenuation"] = _lights_attenuation;
	}

	if (shader_program.HasUniform("_lights_spot_attenuation")) {
		shader_program["_lights_spot_attenuation"] = _lights_spot_attenuation;
	}

	if (shader_program.HasUniform("_lightCount")) {
		shader_program["_lightCount"] = (GLint) _lights_type.size();
	}

	if (shader_program.HasUniform("_shadowLevel")) {
		shader_program["_shadowLevel"] = (GLint) _shadow_level;
	}
}

gl::Program& SceneRenderManager::GetForwardModelShader() {
	return _model_shaders->forward_model_shader;
}

gl::Program& SceneRenderManager::GetOpaqueShader() {
	return _model_shaders->opaque_shader;
}

int SceneRenderManager::_get_shadow_quality() {
	if (std::ranges::any_of(_scene->GetRegistry().GetComponents<PointLight, SpotLight, DirectionalLight>().As<Light>(),
			[](const Light& light) { return light.CastsShadows(); })) {
		return DisplayConfiguration::Get().shadow_quality;
	}

	return 0;
}

}
