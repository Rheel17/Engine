#include "SceneRenderManager.h"

#include "DeferredSceneRenderer.h"
#include "ForwardSceneRenderer.h"
#include "ShadowMapDirectional.h"
#include "../PointLight.h"
#include "../SpotLight.h"
#include "../DirectionalLight.h"
#include "../Engine.h"
#include "../Resources.h"

#include <unordered_set>
#include "ModelRenderer.h"

namespace rheel {

GLShaderProgram SceneRenderManager::_deferred_lighting_shader;
std::shared_ptr<GLVertexArray> SceneRenderManager::_lighting_quad_vao(nullptr);
std::shared_ptr<GLBuffer> SceneRenderManager::_lighting_quad_vbo(nullptr);
bool SceneRenderManager::_lighting_quad_initialized = false;

SceneRenderManager::SceneRenderManager(Scene_t *scene) :
		_scene(scene) {

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

	for (const std::string& lightName : _scene->Lights()) {
		LightPtr light = _scene->GetLight(lightName);

		_lights_color.push_back(light->Color());

		if (auto pointLight = std::dynamic_pointer_cast<PointLight>(light)) {
			_lights_type.push_back(0);
			_lights_position.push_back(pointLight->Position());
			_lights_direction.push_back(vec3());
			_lights_attenuation.push_back(pointLight->Attenuation());
			_lights_spot_attenuation.push_back(0.0f);
		} else if (auto spotLight = std::dynamic_pointer_cast<SpotLight>(light)) {
			_lights_type.push_back(1);
			_lights_position.push_back(spotLight->Position());
			_lights_direction.push_back(spotLight->Direction());
			_lights_attenuation.push_back(spotLight->Attenuation());
			_lights_spot_attenuation.push_back(spotLight->SpotAttenuation());
		} else if (auto directionalLight = std::dynamic_pointer_cast<DirectionalLight>(light)) {
			_lights_type.push_back(2);
			_lights_position.push_back(vec3());
			_lights_direction.push_back(directionalLight->Direction());
			_lights_attenuation.push_back(0.0f);
			_lights_spot_attenuation.push_back(0.0f);
		}
	}

	_shadow_level = _ShadowLevel();
}

ModelRenderer& SceneRenderManager::GetModelRenderer(ModelPtr model) {
	auto iter = _render_map.find(model);

	if (iter == _render_map.end()) {
		iter = _render_map.emplace(model, model).first;
	}

	return iter->second;
}

std::shared_ptr<SceneRenderer> SceneRenderManager::CreateSceneRenderer(std::string cameraName, unsigned width, unsigned height) {
	SceneRenderer *renderer;

	switch (Engine::GetDisplayConfiguration().render_mode) {
		case DisplayConfiguration::FORWARD:
			renderer = new ForwardSceneRenderer(this, std::move(cameraName), width, height);
			break;
		case DisplayConfiguration::DEFERRED:
			renderer = new DeferredSceneRenderer(this, std::move(cameraName), width, height);
			break;
	}

	return std::shared_ptr<SceneRenderer>(renderer);
}

std::shared_ptr<ShadowMap> SceneRenderManager::CreateShadowMap(const std::string& lightName) {
	LightPtr light = _scene->GetLight(lightName);

	if (auto pointLight = std::dynamic_pointer_cast<PointLight>(light)) {

	} else if (auto spoLight = std::dynamic_pointer_cast<SpotLight>(light)) {

	} else if (auto directionalLight = std::dynamic_pointer_cast<DirectionalLight>(light)) {
		return std::shared_ptr<ShadowMapDirectional>(new ShadowMapDirectional(this, light));
	}

	return nullptr;
}

SceneRenderManager::Scene_t *SceneRenderManager::Scene() {
	return _scene;
}

const std::unordered_map<ModelPtr, ModelRenderer>& SceneRenderManager::RenderMap() const {
	return _render_map;
}

GLShaderProgram& SceneRenderManager::InitializedDeferredLightingShader() const {
	InitializeShaderLights(_deferred_lighting_shader);
	return _deferred_lighting_shader;
}

void SceneRenderManager::InitializeShaderLights(GLShaderProgram& shaderProgram) const {
	shaderProgram["lights_type"] = _lights_type;
	shaderProgram["lights_position"] = _lights_position;
	shaderProgram["lights_direction"] = _lights_direction;
	shaderProgram["lights_color"] = _lights_color;
	shaderProgram["lights_attenuation"] = _lights_attenuation;
	shaderProgram["lights_spot_attenuation"] = _lights_spot_attenuation;
	shaderProgram["lightCount"] = (GLint) _lights_type.size();
	shaderProgram["shadowLevel"] = (GLint) _shadow_level;
}

void SceneRenderManager::DrawDeferredLightingQuad() const {
	_lighting_quad_vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

int SceneRenderManager::_ShadowLevel() {
	for (const std::string& light : _scene->Lights()) {
		if (_scene->GetLight(light)->CastsShadows()) {
			return Engine::GetDisplayConfiguration().shadow_quality;
		}
	}

	return 0;
}

void SceneRenderManager::_Initialize() {
	if (_lighting_quad_initialized) {
		return;
	}

	_deferred_lighting_shader.AddShaderFromSource(GLShaderProgram::VERTEX, RESOURCE_AS_STRING(Shaders_deferred_lightingshader_vert_glsl));
	_deferred_lighting_shader.AddShaderFromSource(GLShaderProgram::FRAGMENT, RESOURCE_AS_STRING(Shaders_deferred_lightingshader_frag_glsl));
	_deferred_lighting_shader.Link();
	_deferred_lighting_shader["gBufferColor"] = 0;
	_deferred_lighting_shader["gBufferPosition"] = 1;
	_deferred_lighting_shader["gBufferNormal"] = 2;
	_deferred_lighting_shader["gBufferAmbient"] = 3;
	_deferred_lighting_shader["gBufferDiffuse"] = 4;
	_deferred_lighting_shader["gBufferSpecular"] = 5;
	_deferred_lighting_shader["gBufferMaterialParameters"] = 6;
	_deferred_lighting_shader["shadowMap0"] = 7;
	_deferred_lighting_shader["shadowMap1"] = 8;
	_deferred_lighting_shader["shadowMap2"] = 9;
	_deferred_lighting_shader["shadowMap3"] = 10;

	GLfloat triangles[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
	_lighting_quad_vbo = std::make_shared<GLBuffer>(GL::BufferTarget::ARRAY);
	_lighting_quad_vbo->SetData(triangles, sizeof(triangles));

	_lighting_quad_vao = std::make_shared<GLVertexArray>();
	_lighting_quad_vao->SetVertexAttributes<vec2>(*_lighting_quad_vbo);

	_lighting_quad_initialized = true;
}

}
