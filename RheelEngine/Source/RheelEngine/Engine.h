/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef ENGINE_H_
#define ENGINE_H_
#include "_common.h"

#include "Game.h"
#include "Audio/AudioManager.h"
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

		// renderer
		std::unordered_map<Scene *, SceneRenderManager> render_map_scene;

		// audio
		std::unique_ptr<AudioManager> audio_manager;

		// game state
		Scene *active_scene = nullptr;

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
	 * Returns the audio manager in use.
	 */
	static AudioManager& GetAudioManager();

	/**
	 * Sets the current active scene. The engine will take ownership of the
	 * pointer passed, so do not pass a managed pointer. If exists, the previous
	 * scene will be deleted. Use SetActiveScreen(nullptr) to remove the current
	 * scene.
	 */
	static void SetActiveScene(Scene *scene);

	/**
	 * Returns the current active scene.
	 */
	static Scene *GetActiveScene();

	/**
	 * Returns a SceneRenderManager instance to render the specified scene.
	 * Multiple calls with the same scene will result in the same render
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
