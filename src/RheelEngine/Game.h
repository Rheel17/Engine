/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_GAME_H
#define RHEELENGINE_GAME_H
#include "_common.h"

#include "Scene.h"
#include "ThreadPool.h"
#include "Assets/AssetLoader.h"
#include "Audio/AudioManager.h"
#include "UI/UI.h"
#include "Renderer/ImageTexture.h"
#include "Renderer/SceneRenderManager.h"
#include "Renderer/Display/MainWindow.h"

namespace rheel {

class RE_API Game {
	RE_NO_COPY(Game)
	RE_NO_MOVE(Game)

public:
	Game(DisplayConfiguration displayConfiguration, const std::string& title = "Rheel Game Engine");
	virtual ~Game();

	/**
	 * Called when the game is started. At this point, a window and OpenGL
	 * context are present. This is the final method called before the main game
	 * loop.
	 */
	virtual void Start() = 0;

	/**
	 * Returns the display configuration for this game.
	 */
	const DisplayConfiguration& GetDisplayConfiguration() const;

	/**
	 * Returns the game window.
	 */
	MainWindow& GetWindow();

	/**
	 * Returns the current UI.
	 */
	UI& GetUI();

	/**
	 * Returns this game's asset loader.
	 */
	AssetLoader& GetAssetLoader();

	/**
	 * Returns the audio manager in use.
	 */
	AudioManager& GetAudioManager();

	/**
	 * Sets the current active scene. The engine will take ownership of the
	 * pointer passed, so do not pass a managed pointer. If exists, the previous
	 * scene will be deleted. Use SetActiveScreen(nullptr) to remove the current
	 * scene.
	 */
	void SetActiveScene(Scene* scene);

	/**
	 * Returns the current active scene.
	 */
	Scene* GetActiveScene();

	/**
	 * Returns this game's thread pool, used for background tasks.
	 */
	ThreadPool& GetThreadPool();

	/**
	 * Preloads the image asset for the given path, and afterwards loads the
	 * texture in the graphics card, so it can be used at render-time. This
	 * method will immediately return and run the initializization in the
	 * background.
	 */
	std::future<void> PreloadTexture(const std::string& path, ImageTexture::WrapType type = ImageTexture::WrapType::WRAP, bool linear = true);

	/**
	 * Preloads an image to the graphics card, so it can be used at render-time.
	 * This method will immediately return and run the initializization in the
	 * background.
	 */
	std::future<void> PreloadTexture(const Image& image, ImageTexture::WrapType type = ImageTexture::WrapType::WRAP, bool linear = true);

private:
	void Loop_();

	DisplayConfiguration _display_configuration;
	MainWindow* _window = nullptr;
	std::unique_ptr<UI> _ui;

	std::unique_ptr<AudioManager> _audio_manager;

	AssetLoader _asset_loader;
	ThreadPool* _thread_pool = nullptr;

	Scene* _active_scene = nullptr;

private:
	template<typename T>
	friend void entry();

};

}

#endif
