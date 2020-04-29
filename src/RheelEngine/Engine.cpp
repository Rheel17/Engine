/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Engine.h"

#include "Renderer/Text/TextRenderer.h"

namespace rheel {

Engine::EngineInstance Engine::_instance;

Engine::EngineInstance::~EngineInstance() {
	delete active_scene;
	delete ui;
	delete window;
}

void Engine::Initialize_() {
	DisplayConfiguration::InitializeGLFW();
	Font::Initialize();
	_instance.audio_manager = std::make_unique<AudioManager>();
}

void Engine::Run_(Game* game) {
	game->Initialize();

	_instance.display_configuration.CalculateActualResolution_();

	_instance.window = new MainWindow(_instance.display_configuration);
	_instance.display_configuration.ClampAnisotropicLevel_();

	_instance.ui = new UI(
			_instance.display_configuration.resolution.x,
			_instance.display_configuration.resolution.y);

	TextRenderer::ResizeBuffer_(
			_instance.display_configuration.resolution.x,
			_instance.display_configuration.resolution.y);

	_instance.thread_pool = new ThreadPool;

	game->Start();

	_instance.window->Loop();
}

void Engine::Terminate_() {
	delete _instance.thread_pool;
	_instance.audio_manager->StopAll_();
	DisplayConfiguration::TerminateGLFW();
}

void Engine::SetDisplayConfiguration(DisplayConfiguration config) {
	_instance.display_configuration = std::move(config);
}

const DisplayConfiguration& Engine::GetDisplayConfiguration() {
	return _instance.display_configuration;
}

MainWindow& Engine::GetWindow() {
	return *_instance.window;
}

UI& Engine::GetUI() {
	return *_instance.ui;
}

AssetLoader& Engine::GetAssetLoader() {
	return _instance.asset_loader;
}

AudioManager& Engine::GetAudioManager() {
	return *_instance.audio_manager;
}

void Engine::SetActiveScene(Scene* scene) {
	delete _instance.active_scene;
	_instance.active_scene = scene;
}

Scene* Engine::GetActiveScene() {
	return _instance.active_scene;
}

SceneRenderManager& Engine::GetSceneRenderManager(Scene* scene) {
	auto iter = _instance.render_map_scene.find(scene);

	if (iter == _instance.render_map_scene.end()) {
		iter = _instance.render_map_scene.emplace(scene, scene).first;
	}

	return iter->second;
}

ThreadPool& Engine::GetThreadPool() {
	return *_instance.thread_pool;
}

std::future<void> Engine::PreloadTexture(const std::string& path, ImageTexture::WrapType type, bool linear) {
	return GetThreadPool().AddTask<void>([=](){
		ImageTexture::Get(GetAssetLoader().png.Load(path), type, linear);
	});
}

std::future<void> Engine::PreloadTexture(const Image& image, ImageTexture::WrapType type, bool linear) {
	return GetThreadPool().AddTask<void>([=](){
		ImageTexture::Get(image, type, linear);
	});
}

}
