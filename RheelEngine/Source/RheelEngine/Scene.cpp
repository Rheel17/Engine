#include "Scene.h"

#include "Engine.h"
#include "PerspectiveCamera.h"
#include "Renderer/SceneRenderManager.h"
#include "Scripts/InputScript.h"

namespace rheel {

Scene::Scene(const SceneDescription& description) {
	// first: add the scripts
	for (const auto& script : description.Scripts()) {
		AddScript(script);
	}

	// second: add the objects
	for (const auto& objectDescription : description.ObjectDescriptions()) {
		AddObject(objectDescription.blueprint,
				objectDescription.position,
				objectDescription.rotation,
				objectDescription.scale);
	}

	// third: add the lights
	for (const auto& lightDescription : description.LightDescriptions()) {
		switch (lightDescription.type) {
			case SceneDescription::LightDescription::POINT_LIGHT:
				AddPointLight(
						lightDescription.name,
						lightDescription.position,
						lightDescription.color,
						lightDescription.attenuation);
				break;
			case SceneDescription::LightDescription::SPOT_LIGHT:
				AddSpotLight(
						lightDescription.name,
						lightDescription.position,
						lightDescription.color,
						lightDescription.direction,
						lightDescription.spot_attenuation,
						lightDescription.attenuation);
				break;
			case SceneDescription::LightDescription::DIRECTIONAL_LIGHT:
				AddDirectionalLight(
						lightDescription.name,
						lightDescription.color,
						lightDescription.direction);
				break;
		}
	}

	// fourth: add the cameras
	for (const auto& cameraDescription : description.CameraDescriptions()) {
		switch (cameraDescription.type) {
			case SceneDescription::CameraDescription::PERSPECTIVE_CAMERA:
				AddCamera(
						cameraDescription.perspective_fov,
						cameraDescription.perspective_near,
						cameraDescription.perspective_far,
						cameraDescription.name,
						cameraDescription.position,
						cameraDescription.rotation);
				break;
		}
	}
}

void Scene::AddScript(const std::string& script) {
	ScriptPtr instance = Engine::CreateScript(script);
	instance->_parent_scene = this;
	_scripts.push_back(instance);
}

const std::vector<ScriptPtr>& Scene::Scripts() const {
	return _scripts;
}

void Scene::AddObject(const std::string& blueprintName, const vec3& position, const quat& rotation, const vec3& scale) {
	const Blueprint& blueprint = Engine::GetBlueprint(blueprintName);
	Object& object = _objects.emplace_back(blueprint);
	object._SetParentScene(this);

	object.SetPosition(position);
	object.SetRotation(rotation);
	object.SetScale(scale);

	object.FireEvent(Object::ON_ADD);
}

void Scene::RemoveObject(ObjectPtr ptr) {
	Object *object = *ptr._ptr;
	object->FireEvent(Object::ON_REMOVE);

	size_t index = (((size_t) object) - ((size_t) &_objects.front())) / sizeof(Object);

	if (index >= _objects.size()) {
		throw std::invalid_argument("index of object out of range");
	}

	_objects.erase(_objects.begin() + index);
}

void Scene::AddPointLight(const std::string& name, vec3 position, Color color, float attenuation) {
	Light& light = _lights.emplace_back(std::move(position), std::move(color), attenuation);
	_light_map.insert({ name, &light });
}

Light *Scene::AddPointLight(vec3 position, Color color, float attenuation) {
	Light& light = _lights.emplace_back(std::move(position), std::move(color), attenuation);
	return &light;
}

void Scene::AddSpotLight(const std::string& name, vec3 position, Color color, vec3 direction, float spotAttenuation, float attenuation) {
	Light& light = _lights.emplace_back(std::move(position), std::move(color), std::move(direction), spotAttenuation, attenuation);
	_light_map.insert({ name, &light });
}

Light *Scene::AddSpotLight(vec3 position, Color color, vec3 direction, float spotAttenuation, float attenuation) {
	Light& light = _lights.emplace_back(std::move(position), std::move(color), std::move(direction), spotAttenuation, attenuation);
	return &light;
}

void Scene::AddDirectionalLight(const std::string& name, Color color, vec3 direction) {
	Light& light = _lights.emplace_back(std::move(color), glm::normalize(std::move(direction)));
	_light_map.insert({ name, &light });
}

Light *Scene::AddDirectionalLight(Color color, vec3 direction) {
	Light& light = _lights.emplace_back(std::move(color), std::move(direction));
	return &light;
}

void Scene::AddCamera(float fov, float near, float far, std::string name, vec3 position, quat rotation) {
	_cameras[name] = std::make_shared<PerspectiveCamera>(name, std::move(position), std::move(rotation), fov, near, far);
}

Light *Scene::GetLight(const std::string& lightName) {
	auto iter = _light_map.find(lightName);
	if (iter == _light_map.end()) {
		return nullptr;
	}

	return iter->second;
}

CameraPtr Scene::GetCamera(const std::string& cameraName) {
	auto iter = _cameras.find(cameraName);
	if (iter == _cameras.end()) {
		return nullptr;
	}

	return iter->second;
}

const std::vector<Light>& Scene::Lights() const {
	return _lights;
}

void Scene::Update() {
	// pre-update the scripts
	for (auto script : _scripts) {
		script->PreOnUpdate();
	}

	// update the objects
	for (auto& object : _objects) {
		object.FireEvent(Object::ON_UPDATE);
	}

	// post-update the scripts
	for (auto script : _scripts) {
		script->PostOnUpdate();
	}

	// update the object renderers
	for (auto& object : _objects) {
		object.FireEvent(Object::ON_UPDATE_RENDERER);
	}

	SceneRenderManager& renderManager = Engine::GetSceneRenderManager(this);
	renderManager.Update();

	// reset the input scripts
	for (auto script : _scripts) {
		if (auto inputScript = std::dynamic_pointer_cast<InputScript>(script)) {
			inputScript->_ResetDeltas();
		}
	}
}

}
