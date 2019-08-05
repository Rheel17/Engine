#include "SceneDescription.h"

#include "Engine.h"

namespace rheel {

SceneDescription::CameraDescription::CameraDescription(std::string name, vec3 position, quat rotation, float fov, float near, float far) :
		type(PERSPECTIVE_CAMERA), name(std::move(name)),
		position(std::move(position)), rotation(std::move(rotation)),
		perspective_fov(fov), perspective_near(near), perspective_far(far) {}

void SceneDescription::AddScript(std::string script) {
	if (!Engine::HasScript(script)) {
		throw std::runtime_error("Script not registered: \"" + script + "\"");
	}

	_scripts.push_back(std::move(script));
}

void SceneDescription::AddObject(std::string blueprint, vec3 position, quat rotation, vec3 scale) {
	if (!Engine::HasBlueprint(blueprint)) {
		throw std::runtime_error("Blueprint not registered: \"" + blueprint + "\"");
	}

	_objects.push_back({ std::move(blueprint), std::move(position), std::move(rotation), std::move(scale) });
}

void SceneDescription::AddPointLight(std::string name, vec3 position, Color color, float attenuation) {
	_lights.push_back(LightDescription { LightDescription::POINT_LIGHT, std::move(name), std::move(position), {}, std::move(color), attenuation, 0 });
}

void SceneDescription::AddSpotLight(std::string name, vec3 position, Color color, vec3 direction, float spotAttenuation, float attenuation) {
	_lights.push_back(LightDescription { LightDescription::SPOT_LIGHT, std::move(name), std::move(position), std::move(direction), std::move(color), attenuation, spotAttenuation });
}

void SceneDescription::AddDirectionalLight(std::string name, Color color, vec3 direction) {
	_lights.push_back(LightDescription { LightDescription::DIRECTIONAL_LIGHT, std::move(name), {}, std::move(direction), std::move(color), 0, 0 });
}

void SceneDescription::AddCamera(std::string name, float fov, float near, float far, vec3 position, quat rotation) {
	_cameras.push_back(CameraDescription(std::move(name), std::move(position), std::move(rotation), fov, near, far));
}

}
