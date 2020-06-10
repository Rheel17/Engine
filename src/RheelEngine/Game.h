/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_GAME_H
#define RHEELENGINE_GAME_H
#include "_common.h"

#include "ThreadPool.h"
#include "Assets/AssetLoader.h"
#include "Audio/AudioManager.h"
#include "Renderer/GameRenderer.h"
#include "Renderer/ImageTexture.h"
#include "Renderer/SceneRenderManager.h"
#include "Renderer/Display/MainWindow.h"

namespace rheel {

class Scene;
class UI;

struct RE_API scene_deleter {
	void operator()(Scene* scene);
};

using ScenePointer = std::unique_ptr<Scene, scene_deleter>;

class RE_API Game {
	RE_NO_COPY(Game);
	RE_NO_MOVE(Game);

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
	 * Stops the game.
	 */
	void Stop();

	/**
	 * Returns the game window.
	 */
	MainWindow& GetWindow();

	/**
	 * Returns the current UI.
	 */
	UI& GetUI();

	/**
	 * Returns this game's renderer.
	 */
	GameRenderer& GetRenderer();

	/**
	 * Returns this game's asset loader.
	 */
	AssetLoader& GetAssetLoader();

	/**
	 * Returns the audio manager in use.
	 */
	AudioManager& GetAudioManager();

	/**
	 * Creates a new scene for this game.
	 */
	ScenePointer CreateScene();

	/**
	 * Sets the current active scene. The engine will take ownership of the
	 * pointer passed. If exists, the previous active scene will be deleted. Use
	 * SetActiveScreen(nullptr) to remove the current scene.
	 */
	void SetActiveScene(ScenePointer scene);

	/**
	 * Returns the current active scene, or nullptr if there is no current
	 * active scene.
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

	MainWindow* _window = nullptr;
	UI* _ui;
	GameRenderer* _renderer;

	std::unique_ptr<AudioManager> _audio_manager;

	AssetLoader _asset_loader;
	ThreadPool* _thread_pool = nullptr;

	ScenePointer _active_scene = nullptr;

private:
	template<typename T>
	friend void entry();

};

}

#endif
