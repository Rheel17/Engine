/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef SCENEDESCRIPTION_H_
#define SCENEDESCRIPTION_H_
#include "_common.h"

#include "Blueprint.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Script.h"
#include "SpotLight.h"

namespace rheel {

class RE_API SceneDescription {
	friend class SceneOld;

private:
	struct _ObjectDescription {
		Blueprint blueprint;
		vec3 position;
		quat rotation;
	};

	struct _LightDescription {
		int type;
		std::string name;

		Color color;
		float shadow_distance;

		vec3 position;
		vec3 direction;
		float attenuation;
		float spot_attenuation;

		static constexpr int POINT_LIGHT = 0;
		static constexpr int SPOT_LIGHT = 1;
		static constexpr int DIRECTIONAL_LIGHT = 2;
	};

	struct _CameraDescription {
		int type;
		std::string name;

		vec3 position;
		vec3 rotation;

		float perspective_fov;
		float perspective_near;
		float perspective_far;

		_CameraDescription(std::string name, vec3 position, vec3 rotation, float fov, float near, float far);

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
	 * See:
	 *
	 * Adds a script to the scene description. When a scene is created from this
	 * description, the scripts are the added first, to be able to notify the
	 * scripts of other additions to the scene.
	 */
	template<typename T>
	T& AddScript() {
		static_assert(std::is_base_of<Script, T>::value, "Class is not derived from Script");
		static_assert(std::is_default_constructible<T>::value, "Scripts must be Default-Constructable");

		std::unique_ptr<T> ptr = std::make_unique<T>();
		T& ref = *ptr;

		_scripts.push_back(std::move(ptr));
		return ref;
	}

	/**
	 * See: Scene::AddObject(...). When a scene is created from this
	 * description, the objects are added after the scripts and before the
	 * lights.
	 *
	 * This methods returns a reference to the copied blueprint. This can be
	 * used to access components and preset parameters.
	 */
	Blueprint& AddObject(const std::string& blueprint, vec3 position = { 0, 0, 0 }, quat rotation = quat(1, 0, 0, 0));

	/**
	 * See: Scene::AddLight(...). When a scene is created from this description,
	 * the lights are added after the objects and before the cameras.
	 */
	void AddLight(const std::string& name, const PointLight& light, float shadowDistance = 0.0f);

	/**
	 * See: Scene::AddLight(...). When a scene is created from this description,
	 * the lights are added after the objects and before the cameras.
	 */
	void AddLight(const std::string& name, const SpotLight& light, float shadowDistance = 0.0f);

	/**
	 * See: Scene::AddLight(...). When a scene is created from this description,
	 * the lights are added after the objects and before the cameras.
	 */
	void AddLight(const std::string& name, const DirectionalLight& light, float shadowDistance = 0.0f);

	/**
	 * See: Scene::AddCamera(...). When a scene is created from this
	 * description, the cameras are added last.
	 */
	void AddCamera(std::string name, float fov, float near, float far, vec3 position = { 0, 0, 0 }, vec3 rotation = { 0, 0, 0 });

private:
	const std::vector<std::unique_ptr<Script>>& Scripts() const;
	const std::vector<_ObjectDescription>& ObjectDescriptions() const;
	const std::vector<_LightDescription>& LightDescriptions() const;
	const std::vector<_CameraDescription>& CameraDescriptions() const;

	std::string _name;

	std::vector<std::unique_ptr<Script>> _scripts;
	std::vector<_ObjectDescription> _objects;
	std::vector<_LightDescription> _lights;
	std::vector<_CameraDescription> _cameras;


};

}

#endif
