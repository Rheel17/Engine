/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_ENGINE_H
#define RHEELENGINE_ENGINE_H
#include "_common.h"

#include "Game.h"
#include "UI/UI.h"
#include "ThreadPool.h"
#include "Assets/AssetLoader.h"
#include "Audio/AudioManager.h"
#include "Renderer/ImageTexture.h"
#include "Renderer/SceneRenderManager.h"
#include "Renderer/Window.h"

namespace rheel {

class RE_API Engine {
	struct EngineInstance {
		RE_NO_COPY(EngineInstance);
		RE_NO_MOVE(EngineInstance);
		EngineInstance() = default;

		// assets
		AssetLoader asset_loader;

		// display
		DisplayConfiguration display_configuration;
		Window* window = nullptr;
		UI* ui = nullptr;

		// renderer
		std::unordered_map<Scene*, SceneRenderManager> render_map_scene;

		// audio
		std::unique_ptr<AudioManager> audio_manager;

		// game state
		Scene* active_scene = nullptr;

		// thread pool
		ThreadPool* thread_pool;

		~EngineInstance();
	};

public:
	RE_NO_CONSTRUCT(Engine);

	/**
	 * Sets the configuration to use when creating the display (window)
	 */
	static void SetDisplayConfiguration(DisplayConfiguration config);

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
	 * Returns the global asset loader
	 */
	static AssetLoader& GetAssetLoader();

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
	static void SetActiveScene(Scene* scene);

	/**
	 * Returns the current active scene.
	 */
	static Scene* GetActiveScene();

	/**
	 * Returns a SceneRenderManager instance to render the specified scene.
	 * Multiple calls with the same scene will result in the same render
	 * manager.
	 */
	static SceneRenderManager& GetSceneRenderManager(Scene* scene);

	/**
	 * Returns the global thread pool, used for background tasks.
	 */
	static ThreadPool& GetThreadPool();

	/**
	 * Preloads the image asset for the given path, and afterwards loads the
	 * texture in the graphics card, so it can be used at render-time. This
	 * method will immediately return and run the initializization in the
	 * background.
	 */
	static std::future<void> PreloadTexture(const std::string& path, ImageTexture::WrapType type = ImageTexture::WrapType::WRAP, bool linear = true);

	/**
	 * Preloads an image to the graphics card, so it can be used at render-time.
	 * This method will immediately return and run the initializization in the
	 * background.
	 */
	static std::future<void> PreloadTexture(const Image& image, ImageTexture::WrapType type = ImageTexture::WrapType::WRAP, bool linear = true);

private:
	// Engine and Game life-cycle functions

	static void Initialize_();
	static void Run_(Game* game);
	static void Terminate_();

	static EngineInstance _instance;

	template<typename T>
	friend void entry();

};

}

#endif
