#include "Scene.h"

#include "Engine.h"
#include "PerspectiveCamera.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "Renderer/SceneRenderManager.h"
#include "Scripts/InputScript.h"

namespace rheel {

Scene::Scene(const SceneDescription& description) {
	// first: add the scripts
	for (const auto& script : description.Scripts()) {
		std::unique_ptr<Script> scriptCopy(script->__CloneHeap());
		Script& ref = *scriptCopy;

		_scripts.push_back(std::move(scriptCopy));
		ref._parent_scene = this;
		ref.Initialize();
	}

	// second: add the objects
	for (const auto& objectDescription : description.ObjectDescriptions()) {
		_AddObject(objectDescription);
	}

	// third: add the lights
	for (const auto& lightDescription : description.LightDescriptions()) {
		switch (lightDescription.type) {
			case SceneDescription::_LightDescription::POINT_LIGHT:
				AddLight(lightDescription.name, PointLight(
						lightDescription.color,
						lightDescription.position,
						lightDescription.attenuation), lightDescription.shadow_distance);
				break;
			case SceneDescription::_LightDescription::SPOT_LIGHT:
				AddLight(lightDescription.name, SpotLight(
						lightDescription.color,
						lightDescription.position,
						lightDescription.direction,
						lightDescription.spot_attenuation,
						lightDescription.attenuation), lightDescription.shadow_distance);
				break;
			case SceneDescription::_LightDescription::DIRECTIONAL_LIGHT:
				AddLight(lightDescription.name, DirectionalLight(
						lightDescription.color,
						lightDescription.direction), lightDescription.shadow_distance);
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

const std::vector<std::unique_ptr<Script>>& Scene::Scripts() const {
	return _scripts;
}

void Scene::AddObject(const std::string& blueprintName, const vec3& position, const quat& rotation, const vec3& scale) {
	SceneDescription::_ObjectDescription object = SceneDescription::_ObjectDescription {
			Engine::GetBlueprint(blueprintName), position, rotation, scale
	};

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

Light *Scene::GetLight(const std::string& lightName) {
	auto iter = _lights.find(lightName);
	if (iter == _lights.end()) {
		return nullptr;
	}

	return iter->second.get();
}

const std::vector<std::string>& Scene::Lights() const {
	return _light_names;
}

void Scene::AddCamera(float fov, float near, float far, std::string name, vec3 position, vec3 rotation) {
	_cameras[name] = std::make_unique<PerspectiveCamera>(name, std::move(position), std::move(rotation), fov, near, far);
}

Camera *Scene::GetCamera(const std::string& cameraName) {
	auto iter = _cameras.find(cameraName);
	if (iter == _cameras.end()) {
		return nullptr;
	}

	return iter->second.get();
}

void Scene::Update(float dt) {
	// add the time
	_time += dt;

	// pre-update the scripts
	for (auto& script : _scripts) {
		script->_dt = dt;
		script->_time = _time;
		script->PreOnUpdate();
	}

	// update the objects
	for (auto& object : _objects) {
		object.FireEvent(Object::ON_UPDATE);
	}

	// post-update the scripts
	for (auto& script : _scripts) {
		script->PostOnUpdate();
	}

	// update the object renderers
	for (auto& object : _objects) {
		object.FireEvent(Object::ON_UPDATE_RENDERER);
	}

	SceneRenderManager& renderManager = Engine::GetSceneRenderManager(this);
	renderManager.Update();

	// reset the input scripts
	for (auto& script : _scripts) {
		if (auto inputScript = dynamic_cast<InputScript *>(script.get())) {
			inputScript->_ResetDeltas();
		}
	}
}

void Scene::_AddObject(const SceneDescription::_ObjectDescription& description) {
	Object& object = _objects.emplace_back(description.blueprint);
	object._SetParentScene(this);

	object.SetPosition(description.position);
	object.SetRotation(description.rotation);
	object.SetScale(description.scale);

	object.FireEvent(Object::ON_ADD);
}

void Scene::_AddLight(const std::string& name, Light *light) {
	if (_lights.find(name) != _lights.end()) {
		throw std::runtime_error("Light with name \"" + name + "\" already exists.");
	}

	_lights[name] = std::unique_ptr<Light>(light);
	_light_names.push_back(name);
}

}
