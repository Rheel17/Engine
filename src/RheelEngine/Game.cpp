/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Game.h"

#include "Scene.h"
#include "UI/UI.h"

namespace rheel {

void scene_deleter::operator()(Scene* scene) {
	delete scene;
}

Game::Game(DisplayConfiguration displayConfiguration, const std::string& windowTitle) {
	// initialize the engine
	DisplayConfiguration::InitializeGLFW();
	Font::Initialize();

	// create the window
	displayConfiguration.CalculateActualResolution_();
	_window = new MainWindow(displayConfiguration, windowTitle, *this);

	// update the display configuration
	displayConfiguration.ClampAnisotropicLevel_();
	DisplayConfiguration::Set_(std::move(displayConfiguration));

	// initialize the window contents
	_ui = new UI(*this);
	_renderer = new GameRenderer;

	// initialize the audio manager
	_audio_manager = std::make_unique<AudioManager>();

	// spool up the thread pool
	_thread_pool = new ThreadPool(*_window);

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
	FontRenderer::_renderers.Clear();
	delete _ui;
	delete _renderer;
	delete _window;

	// terminate the engine
	DisplayConfiguration::TerminateGLFW();
}

void Game::Stop() {
	_window->Close();
}

MainWindow& Game::GetWindow() {
	return *_window;
}

UI& Game::GetUI() {
	return *_ui;
}

GameRenderer& Game::GetRenderer() {
	return *_renderer;
}

AssetLoader& Game::GetAssetLoader() {
	return _asset_loader;
}

AudioManager& Game::GetAudioManager() {
	return *_audio_manager;
}

ScenePointer Game::CreateScene() {
	return ScenePointer(new Scene(*this));
}

void Game::SetActiveScene(ScenePointer scene) {
	_active_scene = std::move(scene);
}

Scene* Game::GetActiveScene() {
	return _active_scene.get();
}

ThreadPool& Game::GetThreadPool() {
	return *_thread_pool;
}

std::future<void> Game::PreloadTexture(const std::string& path, ImageTexture::WrapType type, bool linear) {
	return GetThreadPool().AddTask<void>([=, this](){
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
