#ifndef SCENE_H_
#define SCENE_H_
#include "_common.h"

#include <map>

#include "Camera.h"
#include "SceneDescription.h"
#include "Object.h"
#include "Light.h"
#include "Script.h"

namespace rheel {

class RE_API Scene {
	friend class Engine;

public:
	/**
	 * Adds a script to the scene. The script will go into effect immediately. A
	 * pointer to the script is returned.
	 */
	ScriptPtr AddScript(const std::string& script);

	/**
	 * Returns a shared pointer to the script of the given type. A nullptr
	 * shared pointer is returned if no such script exists in this object.
	 */
	template<typename T>
	std::shared_ptr<T> GetScript() {
		static_assert(std::is_base_of<Script, T>::value, "Type must be a script");

		for (auto script : _scripts) {
			if (auto ptr = std::dynamic_pointer_cast<T>(script)) {
				return ptr;
			}
		}

		return nullptr;
	}

	/**
	 * Returns all the scripts currently in this scene
	 */
	const std::vector<ScriptPtr>& Scripts() const;

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
	 * Adds a named point light to the scene.
	 */
	void AddPointLight(const std::string& name, vec3 position, Color color, float attenuation = 0.0f);

	/**
	 * Adds an unnamed point light to the scene. Returns a pointer to the
	 * generated light.
	 */
	Light *AddPointLight(vec3 position, Color color, float attenuation = 0.0f);

	/**
	 * Adds a named spot light to the scene.
	 */
	void AddSpotLight(const std::string& name, vec3 position, Color color, vec3 direction, float spotAttenuation = 1.0f, float attenuation = 0.0f);

	/**
	 * Adds an unnamed spot light to the scene. Returns a pointer to the
	 * generated light.
	 */
	Light *AddSpotLight(vec3 position, Color color, vec3 direction, float spotAttenuation = 0.0f, float attenuation = 0.0f);

	/**
	 * Adds a named directional (sun-like) light to the scene.
	 */
	void AddDirectionalLight(const std::string& name, Color color, vec3 direction);

	/**
	 * Adds an unnamed directional (sun-like) light to the scene. Returns a
	 * pointer to the generated light.
	 */
	Light *AddDirectionalLight(Color color, vec3 direction);

	/**
	 * Returns a pointer to the light in this scene with the given name, or
	 * nullptr when a light with the given name does not exist in this scene.
	 */
	Light *GetLight(const std::string& lightName);

	/**
	 * Returns a vector of all lights in the scene.
	 */
	const std::vector<Light>& Lights() const;

	/**
	 * Adds a camera to the scene with the given camera parameters
	 */
	void AddCamera(float fov, float near, float far, std::string name, vec3 position = { 0, 0, 0 }, quat rotation = quat(1, 0, 0, 0));

	/**
	 * Returns the camera in this scene with the given name, or nullptr when a
	 * camera with the given name does not exist in this scene.
	 */
	CameraPtr GetCamera(const std::string& cameraName);

	/**
	 * Updates the scene and all the objects in the scene.
	 */
	void Update();

private:
	Scene() = default;
	Scene(const SceneDescription& description);

	std::vector<Object> _objects;
	std::vector<ScriptPtr> _scripts;
	std::vector<Light> _lights;
	std::map<std::string, Light *> _light_map;
	std::map<std::string, CameraPtr> _cameras;

};

}

#endif
