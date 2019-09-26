/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "SceneDescription.h"

#include "Engine.h"

namespace rheel {

SceneDescription::_CameraDescription::_CameraDescription(std::string name, vec3 position, vec3 rotation, float fov, float near, float far) :
		type(PERSPECTIVE_CAMERA), name(std::move(name)),
		position(std::move(position)), rotation(std::move(rotation)),
		perspective_fov(fov), perspective_near(near), perspective_far(far) {}

Blueprint& SceneDescription::AddObject(const std::string& blueprint, vec3 position, quat rotation) {
	if (!Engine::HasBlueprint(blueprint)) {
		throw std::runtime_error("Blueprint not registered: \"" + blueprint + "\"");
	}

	_ObjectDescription& description = _objects.emplace_back(_ObjectDescription { Engine::GetBlueprint(blueprint), std::move(position), std::move(rotation) } );
	return description.blueprint;
}

void SceneDescription::AddLight(const std::string& name, const PointLight& light, float shadowDistance) {
	_lights.push_back(_LightDescription { _LightDescription::POINT_LIGHT, name, light.GetColor(), shadowDistance, light.Position(), vec3(), light.Attenuation(), 0.0f });
}

void SceneDescription::AddLight(const std::string& name, const SpotLight& light, float shadowDistance) {
	_lights.push_back(_LightDescription { _LightDescription::SPOT_LIGHT, name, light.GetColor(), shadowDistance, light.Position(), light.Direction(), light.Attenuation(), light.SpotAttenuation() });
}

void SceneDescription::AddLight(const std::string& name, const DirectionalLight& light, float shadowDistance) {
	_lights.push_back(_LightDescription { _LightDescription::DIRECTIONAL_LIGHT, name, light.GetColor(), shadowDistance, vec3(), light.Direction(), 0.0f, 0.0f });
}

void SceneDescription::AddCamera(std::string name, float fov, float near, float far, vec3 position, vec3 rotation) {
	_cameras.push_back(_CameraDescription(std::move(name), std::move(position), std::move(rotation), fov, near, far));
}

const std::vector<std::unique_ptr<Script>>& SceneDescription::Scripts() const {
	return _scripts;
}

const std::vector<SceneDescription::_ObjectDescription>& SceneDescription::ObjectDescriptions() const {
	return _objects;
}

const std::vector<SceneDescription::_LightDescription>& SceneDescription::LightDescriptions() const {
	return _lights;
}

const std::vector<SceneDescription::_CameraDescription>& SceneDescription::CameraDescriptions() const {
	return _cameras;
}

}
