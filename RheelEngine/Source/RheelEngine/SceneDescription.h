#ifndef SCENEDESCRIPTION_H_
#define SCENEDESCRIPTION_H_
#include "_common.h"

#include <string>
#include <vector>

#include "Color.h"

namespace rheel {

class RE_API SceneDescription {
	friend class Scene;

	struct ObjectDescription {
		std::string blueprint;
		vec3 position;
		quat rotation;
		vec3 scale;
	};

	struct LightDescription {
		int type;
		std::string name;

		vec3 position;
		vec3 direction;
		Color color;
		float attenuation;
		float spot_attenuation;

		static constexpr int POINT_LIGHT = 0;
		static constexpr int SPOT_LIGHT = 1;
		static constexpr int DIRECTIONAL_LIGHT = 2;
	};

	struct CameraDescription {
		int type;
		std::string name;

		vec3 position;
		vec3 rotation;

		float perspective_fov;
		float perspective_near;
		float perspective_far;

		CameraDescription(std::string name, vec3 position, vec3 rotation, float fov, float near, float far);

		static constexpr int PERSPECTIVE_CAMERA = 0;
	};

public:
	/*
	 * Constructs a scene description object
	 */
	SceneDescription(std::string name) :
			_name(std::move(name)) {}

	/**
	 * Returns the name of this scene description.
	 */
	const std::string& Name() { return _name; }

	/**
	 * Adds a script to the scene description. When a scene is created from this
	 * description, the scripts are the added first, to be able to notify the
	 * scripts of other additions to the scene.
	 */
	void AddScript(std::string script);

	/**
	 * See: Scene::AddObject(...). When a scene is created from this
	 * description, the objects are added after the scripts and before the
	 * lights.
	 */
	void AddObject(std::string blueprint, vec3 position = { 0, 0, 0 }, quat rotation = quat(1, 0, 0, 0), vec3 scale = { 1, 1, 1 });

	/**
	 * See: Scene::AddPointLight(...). When a scene is create from this
	 * description, the lights are added after the objects and before the
	 * cameras.
	 */
	void AddPointLight(std::string name, vec3 position, Color color, float attenuation = 0.0f);

	/**
	 * See: Scene::AddSpotLight(...). When a scene is created from this
	 * description, the lights are added after the objects and before the
	 * cameras.
	 */
	void AddSpotLight(std::string name, vec3 position, Color color, vec3 direction, float spotAttenuation = 1.0f, float attenuation = 0.0f);

	/**
	 * See: Scene::AddDirectionalLight(...). When a scene is created from this
	 * description, the lights are added after the objects and before the
	 * cameras.
	 */
	void AddDirectionalLight(std::string name, Color color, vec3 direction);

	/**
	 * See: Scene::AddCamera(...). When a scene is created from this
	 * description, the cameras are added last.
	 */
	void AddCamera(float fov, float near, float far, std::string name, vec3 position = { 0, 0, 0 }, vec3 rotation = { 0, 0, 0 });

	/**
	 * Returns the vector of script names in this scene description.
	 */
	const std::vector<std::string> Scripts() const { return _scripts; }

	/**
	 * Returns the vector of object descriptions in this scene description.
	 */
	const std::vector<ObjectDescription> ObjectDescriptions() const { return _objects; }

	/**
	 * Returns the vector of light descriptions in this scene description.
	 */
	const std::vector<LightDescription> LightDescriptions() const { return _lights; }

	/**
	 * Returns the vector of camera descriptions in this scene description.
	 */
	const std::vector<CameraDescription> CameraDescriptions() const { return _cameras; }

private:
	std::string _name;
	std::vector<std::string> _scripts;
	std::vector<ObjectDescription> _objects;
	std::vector<LightDescription> _lights;
	std::vector<CameraDescription> _cameras;

};

}

#endif
