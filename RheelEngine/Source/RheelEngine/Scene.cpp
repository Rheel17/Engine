#include "Scene.h"

#include "Engine.h"
#include "PerspectiveCamera.h"
#include "Renderer/SceneRenderManager.h"
#include "Scripts/InputScript.h"

namespace rheel {

Scene::Scene(const SceneDescription& description) {
	// first: add the scripts
	for (const auto& scriptEntry : description.Scripts()) {
		ScriptPtr script = AddScript(scriptEntry.first);
		scriptEntry.second(script);
	}

	// second: add the objects
	for (const auto& objectDescription : description.ObjectDescriptions()) {
		_AddObject(objectDescription);
	}

	// third: add the lights
	for (const auto& lightDescription : description.LightDescriptions()) {
		switch (lightDescription.type) {
			case SceneDescription::_LightDescription::POINT_LIGHT:
				AddPointLight(
						lightDescription.name,
						lightDescription.position,
						lightDescription.color,
						lightDescription.casts_shadows,
						lightDescription.attenuation);
				break;
			case SceneDescription::_LightDescription::SPOT_LIGHT:
				AddSpotLight(
						lightDescription.name,
						lightDescription.position,
						lightDescription.color,
						lightDescription.direction,
						lightDescription.casts_shadows,
						lightDescription.spot_attenuation,
						lightDescription.attenuation);
				break;
			case SceneDescription::_LightDescription::DIRECTIONAL_LIGHT:
				AddDirectionalLight(
						lightDescription.name,
						lightDescription.color,
						lightDescription.direction,
						lightDescription.casts_shadows);
				break;
		}
	}

	// fourth: add the cameras
	for (const auto& cameraDescription : description.CameraDescriptions()) {
		switch (cameraDescription.type) {
			case SceneDescription::_CameraDescription::PERSPECTIVE_CAMERA:
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

Scene::~Scene() {
	for (auto pair : _lights) {
		delete pair.second;
	}
}

ScriptPtr Scene::AddScript(const std::string& script) {
	ScriptPtr instance = Engine::CreateScript(script);
	instance->_parent_scene = this;
	_scripts.push_back(instance);
	return instance;
}

const std::vector<ScriptPtr>& Scene::Scripts() const {
	return _scripts;
}

void Scene::AddObject(const std::string& blueprintName, const vec3& position, const quat& rotation, const vec3& scale) {
	SceneDescription::ObjectDescription object = SceneDescription::ObjectDescription(
			blueprintName, position, rotation, scale
	);

	_AddObject(object);
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

void Scene::AddPointLight(const std::string& name, vec3 position, Color color, bool castsShadows, float attenuation) {
	_AddLight(name, new Light(std::move(position), std::move(color), attenuation, castsShadows));
}

void Scene::AddSpotLight(const std::string& name, vec3 position, Color color, vec3 direction, bool castsShadows, float spotAttenuation, float attenuation) {
	_AddLight(name, new Light(std::move(position), std::move(color), std::move(direction), spotAttenuation, attenuation, castsShadows));
}

void Scene::AddDirectionalLight(const std::string& name, Color color, vec3 direction, bool castsShadows) {
	_AddLight(name, new Light(std::move(color), glm::normalize(std::move(direction)), castsShadows));
}

Light *Scene::GetLight(const std::string& lightName) {
	auto iter = _lights.find(lightName);
	if (iter == _lights.end()) {
		return nullptr;
	}

	return iter->second;
}

const std::vector<std::string>& Scene::Lights() const {
	return _light_names;
}

void Scene::AddCamera(float fov, float near, float far, std::string name, vec3 position, vec3 rotation) {
	_cameras[name] = std::make_shared<PerspectiveCamera>(name, std::move(position), std::move(rotation), fov, near, far);
}

CameraPtr Scene::GetCamera(const std::string& cameraName) {
	auto iter = _cameras.find(cameraName);
	if (iter == _cameras.end()) {
		return nullptr;
	}

	return iter->second;
}

void Scene::Update(float dt) {
	// add the time
	_time += dt;

	// pre-update the scripts
	for (auto script : _scripts) {
		script->_dt = dt;
		script->_time = _time;
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

void Scene::_AddObject(const SceneDescription::ObjectDescription& description) {
	const Blueprint& blueprint = Engine::GetBlueprint(description._blueprint);
	Object& object = _objects.emplace_back(blueprint);
	object._SetParentScene(this);

	object.SetPosition(description._position);
	object.SetRotation(description._rotation);
	object.SetScale(description._scale);

	ObjectPtr ptr;
	ptr = &object;
	description.loader(ptr);

	object.FireEvent(Object::ON_ADD);
}

void Scene::_AddLight(const std::string& name, Light *light) {
	if (_lights.find(name) != _lights.end()) {
		delete light;
		throw std::runtime_error("Light with name \"" + name + "\" already exists.");
	}

	_lights[name] = light;
	_light_names.push_back(name);
}

}
