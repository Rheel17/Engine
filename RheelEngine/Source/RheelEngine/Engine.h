#ifndef ENGINE_H_
#define ENGINE_H_
#include "_common.h"

#include <functional>
#include <unordered_map>

#include "Game.h"
#include "Blueprint.h"
#include "Component.h"
#include "Renderer/SceneRenderManager.h"
#include "Script.h"
#include "Scene.h"
#include "Renderer/Window.h"
#include "UI/UI.h"

namespace rheel {

class RE_API Engine {
	struct EngineInstance {
		// display
		DisplayConfiguration display_configuration;
		std::shared_ptr<Window> window = nullptr;

		// game state
		Scene *active_scene = nullptr;
		ui::UIPtr ui = nullptr;

		// register maps
		std::unordered_map<std::string, Blueprint> register_blueprints;
		std::unordered_map<std::string, SceneDescription> register_scene_descriptions;
		std::unordered_map<std::string, std::function<ComponentPtr()>> register_components;
		std::unordered_map<std::string, std::function<ScriptPtr()>> register_scripts;

		// render maps
		std::unordered_map<Scene *, SceneRenderManager> render_map_scene;

		// active state
		std::vector<Scene *> scenes;

		~EngineInstance();
	};

public:
	/**
	 * Sets the configuration to use when creating the display (window)
	 */
	static void SetDisplayConfiguration(const DisplayConfiguration& config);

	/**
	 * Returns the display configuration.
	 */
	static const DisplayConfiguration& GetDisplayConfiguration();

	/**
	 * Sets the current UI
	 */
	static void SetUI(ui::UIPtr ui);

	/**
	 * Gets the current UI
	 */
	static ui::UIPtr GetUI();

	/**
	 * Creates a new empty scene.
	 */
	static Scene *CreateScene();

	/**
	 * Constructs a scene from the given scene description. The scene description must
	 * have been previously registered using RegisterSceneDescription(...).
	 */
	static Scene *CreateScene(const std::string& sceneDescription);

	/**
	 * Sets the current active scene from a pointer to a previously instantiated scene.
	 * See also: SetActiveScene(const std::string&) to use a registered scene description
	 */
	static void SetActiveScene(Scene *scene);

	/**
	 * Sets the current active scene from a scene description. The scene description must
	 * have been previously registered using RegisterSceneDescription(...).
	 * See also: SetActiveScene(Scene *) to use a previously instantiated scene.
	 */
	static void SetActiveScene(const std::string& sceneDescription);

	/**
	 * Returns the current active scene.
	 */
	static Scene *GetActiveScene();

	/**
	 * Updates all the scenes.
	 */
	static void UpdateScenes();

	/**
	 * Registers a blueprint for future use.
	 */
	static void RegisterBlueprint(Blueprint blueprint);

	/**
	 * Returns true only if a blueprint with the given name has been
	 * registered.
	 */
	static bool HasBlueprint(const std::string& name);

	/**
	 * Returns the blueprint with the given name.
	 */
	static const Blueprint& GetBlueprint(const std::string& name);

	/**
	 * Registers a scene description for future use.
	 */
	static void RegisterSceneDescription(SceneDescription sceneDescription);

	/**
	 * Returns true only if a scene description with the given name has been
	 * registered.
	 */
	static bool HasSceneDescription(const std::string& name);

	/**
	 * Returns the scene description with the given name.
	 */
	static const SceneDescription& GetSceneDescription(const std::string& name);

	/**
	 * Registers a component for future use.
	 */
	template<typename T>
	static void RegisterComponent(const std::string& name) {
		static_assert(std::is_base_of<Component, T>::value, "Registered component is not derived from Component");
		static_assert(std::is_default_constructible<T>::value, "Components must be Default-Constructable");

		_instance.register_components.insert({ name, []() -> ComponentPtr {
			return std::make_shared<T>();
		} });
	}

	/**
	 * Returns true only if a component with the given name has been
	 * registered.
	 */
	static bool HasComponent(const std::string& name);

	/**
	 * Creates a new component instance of the given name.
	 */
	static ComponentPtr CreateComponent(const std::string& name);

	/**
	 * Registers a script for future use.
	 */
	template<typename T>
	static void RegisterScript(const std::string& name) {
		static_assert(std::is_base_of<Script, T>::value, "Registered script is not derived from Script");
		static_assert(std::is_default_constructible<T>::value, "Scripts must be Default-Constructable");

		_instance.register_scripts.insert({ name, []() -> ScriptPtr {
			return std::make_shared<T>();
		} });
	}

	/**
	 * Return true only if a script with the given name has been
	 * registered.
	 */
	static bool HasScript(const std::string& name);

	/**
	 * Creates a new script instance of the given name.
	 */
	static ScriptPtr CreateScript(const std::string& name);

	/**
	 * Returns a GetSceneRenderManager instance to render the specified
	 * scene. Multiple calls with the same scene will result in the same
	 * scene render manager.
	 */
	static SceneRenderManager& GetSceneRenderManager(Scene *scene);

private:
	// Engine and Game life-cycle functions

	static void _Initialize();
	static void _Run(Game *game);
	static void _Terminate();

	static EngineInstance _instance;

	template<typename T>
	friend void __Entry__();

private:
	Engine();

};

}

#endif
