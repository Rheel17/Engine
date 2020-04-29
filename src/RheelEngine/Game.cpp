/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Game.h"

#include "Renderer/Text/Font.h"

namespace rheel {

Game::Game(DisplayConfiguration displayConfiguration, const std::string& windowTitle) :
		_display_configuration(std::move(displayConfiguration)) {

	// initialize the engine
	DisplayConfiguration::InitializeGLFW();
	Font::Initialize();

	// create the window
	_display_configuration.CalculateActualResolution_();
	_window = new MainWindow(_display_configuration);
	_display_configuration.ClampAnisotropicLevel_();
	_ui = std::make_unique<UI>(_display_configuration.resolution.x, _display_configuration.resolution.y);

	// initialize the audio manager
	_audio_manager = std::make_unique<AudioManager>();

	// spool up the thread pool
	_thread_pool = new ThreadPool;

	// show the window
	_window->SetVisible(true);
}

Game::~Game() {
	// stop all sounds
	_audio_manager->StopAll_();

	// hide the window before closing it to make the shutdown process invisible
	// to the user
	_window->SetVisible(false);

	// delete the current active scene
	SetActiveScene(nullptr);

	// stop the thread pool
	delete _thread_pool;

	// destroy the window and its contents
	// TODO: destroy all OpenGL objects
	delete _window;

	// terminate the engine
	DisplayConfiguration::TerminateGLFW();
}

const DisplayConfiguration& Game::GetDisplayConfiguration() const {
	return _display_configuration;
}

MainWindow& Game::GetWindow() {
	return *_window;
}

UI& Game::GetUI() {
	return *_ui;
}

AssetLoader& Game::GetAssetLoader() {
	return _asset_loader;
}

AudioManager& Game::GetAudioManager() {
	return *_audio_manager;
}

void Game::SetActiveScene(Scene* scene) {
	delete _active_scene;
	_active_scene = scene;
}

Scene* Game::GetActiveScene() {
	return _active_scene;
}

ThreadPool& Game::GetThreadPool() {
	return *_thread_pool;
}

std::future<void> Game::PreloadTexture(const std::string& path, ImageTexture::WrapType type, bool linear) {
	return GetThreadPool().AddTask<void>([=](){
		ImageTexture::Get(GetAssetLoader().png.Load(path), type, linear);
	});
}

std::future<void> Game::PreloadTexture(const Image& image, ImageTexture::WrapType type, bool linear) {
	return GetThreadPool().AddTask<void>([=](){
		ImageTexture::Get(image, type, linear);
	});
}

void Game::Loop_() {
	_window->Loop();
}

}
