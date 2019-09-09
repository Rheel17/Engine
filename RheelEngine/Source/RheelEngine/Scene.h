#ifndef SCENE_H_
#define SCENE_H_
#include "_common.h"

#include <unordered_map>

#include "Camera.h"
#include "Object.h"
#include "SceneDescription.h"

namespace rheel {

class RE_API Scene {
	friend class Engine;

	RE_NO_COPY(Scene);
	RE_NO_MOVE(Scene);

public:
	/**
	 * Adds a script to the scene. The script will go into effect immediately. A
	 * reference to the script is returned.
	 */
	template<typename T>
	T& AddScript() {
		static_assert(std::is_base_of<Script, T>::value, "Class is not derived from Script");
		static_assert(std::is_default_constructible<T>::value, "Scripts must be Default-Constructable");

		std::unique_ptr<T> ptr = std::make_unique<T>();
		T& ref = *ptr;

		_scripts.push_back(std::move(ptr));
		ref._parent_scene = this;
		ref.Initialize();

		return ref;
	}

	/**
	 * Returns a pointer to the script of the given type. A nullptr is returned
	 * if no such script exists in this object.
	 */
	template<typename T>
	T *GetScript() {
		static_assert(std::is_base_of<Script, T>::value, "Type must be a script");

		for (const auto& script : _scripts) {
			if (auto ptr = dynamic_cast<T *>(script.get())) {
				return ptr;
			}
		}

		return nullptr;
	}

	/**
	 * Returns all the scripts currently in this scene
	 */
	const std::vector<std::unique_ptr<Script>>& Scripts() const;

	/**
	 * Adds an object to the scene with the given blueprint. A position and
	 * rotation can be specified, but if not, the default (origin, no rotation)
	 * are used.
	 */
	void AddObject(const std::string& blueprint, const vec3& position = { 0, 0, 0 }, const quat& rotation = quat(1, 0, 0, 0), const vec3& scale = { 1, 1, 1 });

	/**
	 * Removes an object from this scene.
	 */
	void RemoveObject(ObjectPtr object);

	/**
	 * Adds a light to this scene.
	 */
	template<typename T>
	void AddLight(const std::string& name, const T& light) {
		static_assert(std::is_base_of<Light, T>::value, "Type must be a light");
		_AddLight(name, new T(light));
	}

	/**
	 * Adds a light to this scene. A shadow distance parameter can be specified
	 * with this overload of the method. If the light already had a shadow
	 * distance set, this function will overwrite it.
	 */
	template<typename T>
	void AddLight(const std::string& name, const T& light, float shadowDistance) {
		static_assert(std::is_base_of<Light, T>::value, "Type must be a light");

		T *lightPtr = new T(light);
		lightPtr->SetShadowDistance(shadowDistance);

		_AddLight(name, lightPtr);
	}

	/**
	 * Returns a pointer to the light in this scene with the given name, or
	 * nullptr when a light with the given name does not exist in this scene.
	 */
	Light *GetLight(const std::string& lightName);

	/**
	 * Returns a vector of all lights in the scene.
	 */
	const std::vector<std::string>& Lights() const;

	/**
	 * Adds a camera to the scene with the given camera parameters
	 */
	void AddCamera(float fov, float near, float far, std::string name, vec3 position = { 0, 0, 0 }, vec3 rotation = { 0, 0, 0 });

	/**
	 * Returns the camera in this scene with the given name, or nullptr when a
	 * camera with the given name does not exist in this scene.
	 */
	Camera *GetCamera(const std::string& cameraName);

	/**
	 * Updates the scene and all the objects in the scene.
	 */
	void Update(float dt);

private:
	void _AddObject(const SceneDescription::_ObjectDescription& description);
	void _AddLight(const std::string& name, Light *light);

	Scene() = default;
	Scene(const SceneDescription& description);

	std::vector<Object> _objects;
	std::vector<std::unique_ptr<Script>> _scripts;
	std::vector<std::string> _light_names;
	std::unordered_map<std::string, std::unique_ptr<Light>> _lights;
	std::unordered_map<std::string, std::unique_ptr<Camera>> _cameras;

	float _time = 0.0f;

};

}

#endif
