/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Engine.h"

#include <cstring>

#include "ResourceManager.h"
#include "Renderer/Text/Font.h"

namespace rheel {

Engine::EngineInstance Engine::_instance;

Engine::EngineInstance::~EngineInstance() {
	delete active_scene;
	delete ui;
}

void Engine::_Initialize() {
	Window::InitializeDisplaySystems();
	Font::Initialize();
	_instance.audio_manager = std::make_unique<AudioManager>();
}

void Engine::_Run(Game *game) {
	game->Initialize();

	_instance.display_configuration._CalculateActualResolution();
	_instance.window = new Window(_instance.display_configuration);
	_instance.window->Show();
	_instance.ui = new UI(
				_instance.display_configuration.resolution.width,
				_instance.display_configuration.resolution.height);

	GL::SetWindowFramebufferSize(
			_instance.display_configuration.resolution.width,
			_instance.display_configuration.resolution.height);

	game->Start();

	_instance.window->Loop();
}

void Engine::_Terminate() {
	_instance.audio_manager->_StopAll();
	ResourceManager::_ReleaseAll();
	Window::DestroyDisplaySystems();
}

void Engine::SetDisplayConfiguration(const DisplayConfiguration& config) {
	_instance.display_configuration = config;
}

const DisplayConfiguration& Engine::GetDisplayConfiguration() {
	return _instance.display_configuration;
}

Window& Engine::GetWindow() {
	return *_instance.window;
}

UI& Engine::GetUI() {
	return *_instance.ui;
}

AudioManager& Engine::GetAudioManager() {
	return *_instance.audio_manager;
}

void Engine::SetActiveScene(Scene *scene) {
	delete _instance.active_scene;
	_instance.active_scene = scene;
}

Scene *Engine::GetActiveScene() {
	return _instance.active_scene;
}

SceneRenderManager& Engine::GetSceneRenderManager(Scene *scene) {
	auto iter = _instance.render_map_scene.find(scene);

	if (iter == _instance.render_map_scene.end()) {
		iter = _instance.render_map_scene.emplace(scene, scene).first;
	}

	return iter->second;
}

}
