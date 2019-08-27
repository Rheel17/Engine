#ifndef SCENEDESCRIPTION_H_
#define SCENEDESCRIPTION_H_
#include "_common.h"

#include <functional>
#include <string>
#include <vector>
#include <tuple>

#include "Color.h"
#include "Script.h"
#include "ObjectPtr.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"

namespace rheel {

class RE_API SceneDescription {
	friend class Scene;

	using _ScriptLoader = std::function<void(ScriptPtr)>;
	using _ObjectLoader = std::function<void(ObjectPtr)>;

public:
	class ObjectDescription {
			friend class SceneDescription;
			friend class Scene;

		public:
			ObjectDescription(const std::string& blueprint, const vec3& position, const quat& rotation, const vec3& scale);

			_ObjectLoader loader;

		private:
			std::string _blueprint;
			vec3 _position;
			quat _rotation;
			vec3 _scale;

		};

private:
	struct _LightDescription {
		int type;
		std::string name;

		Color color;
		bool shadow_distance;

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
	 * Adds a script to the scene description. When a scene is created from this
	 * description, the scripts are the added first, to be able to notify the
	 * scripts of other additions to the scene.
	 */
	void AddScript(const std::string& script, _ScriptLoader onLoad = [](ScriptPtr){});

	/**
	 * See: Scene::AddObject(...). When a scene is created from this
	 * description, the objects are added after the scripts and before the
	 * lights.
	 *
	 * This methods returns a reference to the created object description. This
	 * can be used to set parameters. Storing this reference will lead to
	 * invalid memory.
	 */
	SceneDescription::ObjectDescription& AddObject(std::string blueprint, vec3 position = { 0, 0, 0 }, quat rotation = quat(1, 0, 0, 0), vec3 scale = { 1, 1, 1 });

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

	/**
	 * Returns the vector of script names in this scene description.
	 */
	const std::vector<std::pair<std::string, _ScriptLoader>>& Scripts() const;

	/**
	 * Returns the vector of object descriptions in this scene description.
	 */
	const std::vector<ObjectDescription>& ObjectDescriptions() const;

	/**
	 * Returns the vector of light descriptions in this scene description.
	 */
	const std::vector<_LightDescription>& LightDescriptions() const;

	/**
	 * Returns the vector of camera descriptions in this scene description.
	 */
	const std::vector<_CameraDescription>& CameraDescriptions() const;

private:
	std::string _name;

	std::vector<std::pair<std::string, _ScriptLoader>> _scripts;
	std::vector<ObjectDescription> _objects;
	std::vector<_LightDescription> _lights;
	std::vector<_CameraDescription> _cameras;


};

}

#endif
