/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Scene.h"

#include "Engine.h"
#include "PerspectiveCamera.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "Renderer/SceneRenderManager.h"

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

Object& Scene::AddObject(const std::string& blueprintName, const vec3& position, const quat& rotation) {
	SceneDescription::_ObjectDescription object = SceneDescription::_ObjectDescription {
			Engine::GetBlueprint(blueprintName), position, rotation
	};

	return _AddObject(object);
}

void Scene::RemoveObject(Object& ptr) {
	if (_is_iterating_objects) {
		_to_remove_objects.push_back(std::ref(ptr));
		return;
	}

	Object *object = &ptr;
	object->FireEvent(Object::ON_REMOVE);

	auto iter = std::find_if(_objects.begin(), _objects.end(),
			[object](const auto& ptr) { return ptr.get() == object; });
	_objects.erase(iter);
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
		if (script->IsActive()) {
			script->_dt = dt;
			script->_time = _time;
			script->PreOnUpdate();
		}
	}

	// pre-update the objects
	for (const auto& obj : _objects) {
		obj->_dt = dt;
		obj->_time = _time;
	}

	// update the objects
	_FireObjectsEvent(Object::ON_UPDATE);

	// post-update the scripts
	for (auto& script : _scripts) {
		if (script->IsActive()) {
			script->PostOnUpdate();
		}
	}

	// update the object renderers
	_FireObjectsEvent(Object::ON_UPDATE_RENDERER);

	SceneRenderManager& renderManager = Engine::GetSceneRenderManager(this);
	renderManager.Update();

	// reset the input scripts
	for (auto& script : _scripts) {
		if (script->IsActive()) {
			script->_ResetDeltas();
		}
	}
}

Object& Scene::_AddObject(const SceneDescription::_ObjectDescription& description) {
	const auto& objectPtr = _objects.emplace_back(std::make_unique<Object>(description.blueprint));
	objectPtr->_SetParentScene(this);

	objectPtr->SetPosition(description.position);
	objectPtr->SetRotation(description.rotation);

	objectPtr->FireEvent(Object::ON_ADD);
	return *objectPtr;
}

void Scene::_AddLight(const std::string& name, Light *light) {
	if (_lights.find(name) != _lights.end()) {
		throw std::runtime_error("Light with name \"" + name + "\" already exists.");
	}

	_lights[name] = std::unique_ptr<Light>(light);
	_light_names.push_back(name);
}

void Scene::_FireObjectsEvent(Object::EventType event) {
	_is_iterating_objects = true;

	for (auto& object : _objects) {
		object->FireEvent(event);
	}

	_is_iterating_objects = false;

	for (const auto& objectReference : _to_remove_objects) {
		RemoveObject(objectReference.get());
	}

	_to_remove_objects.clear();
}

}
