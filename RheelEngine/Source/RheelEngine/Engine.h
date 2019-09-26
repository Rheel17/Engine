/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef ENGINE_H_
#define ENGINE_H_
#include "_common.h"

#include "Game.h"
#include "Renderer/SceneRenderManager.h"
#include "Renderer/Window.h"
#include "UI/UI.h"

namespace rheel {

class RE_API Engine {
	struct EngineInstance {
		RE_NO_COPY(EngineInstance);
		RE_NO_MOVE(EngineInstance);
		EngineInstance() = default;

		// display
		DisplayConfiguration display_configuration;
		Window *window = nullptr;
		UI *ui = nullptr;

		// game state
		Scene *active_scene = nullptr;

		// register maps
		std::unordered_map<std::string, Blueprint> register_blueprints;
		std::unordered_map<std::string, SceneDescription> register_scene_descriptions;

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
	 * Gets the game window.
	 */
	static Window& GetWindow();

	/**
	 * Gets the current UI
	 */
	static UI& GetUI();

	/**
	 * Creates a new empty scene.
	 */
	static Scene *CreateScene();

	/**
	 * Constructs a scene from the given scene description. The scene
	 * description must have been previously registered using
	 * RegisterSceneDescription(...).
	 */
	static Scene *CreateScene(const std::string& sceneDescription);

	/**
	 * Sets the current active scene from a pointer to a previously instantiated
	 * scene. See also: SetActiveScene(const std::string&) to use a registered
	 * scene description
	 */
	static void SetActiveScene(Scene *scene);

	/**
	 * Sets the current active scene from a scene description. The scene
	 * description must have been previously registered using
	 * RegisterSceneDescription(...). See also: SetActiveScene(Scene *) to use a
	 * previously instantiated scene.
	 */
	static void SetActiveScene(const std::string& sceneDescription);

	/**
	 * Returns the current active scene.
	 */
	static Scene *GetActiveScene();

	/**
	 * Updates all the scenes.
	 */
	static void UpdateScenes(float dt);

	/**
	 * Registers a blueprint for future use.
	 */
	static void RegisterBlueprint(Blueprint blueprint);

	/**
	 * Returns true only if a blueprint with the given name has been registered.
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
	 * Returns a GetSceneRenderManager instance to render the specified scene.
	 * Multiple calls with the same scene will result in the same scene render
	 * manager.
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
