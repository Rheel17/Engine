#include "Engine.h"

#include "Renderer/OpenGL/GLFramebuffer.h"
#include <type_traits>
#include "Renderer/Text/Font.h"

namespace rheel {

Engine::EngineInstance Engine::_instance;

extern void registerScripts();

Engine::EngineInstance::~EngineInstance() {
	for (Scene *scene : scenes) {
		delete scene;
	}

	delete ui;
}

void Engine::_Initialize() {
	Window::InitializeDisplaySystems();
	Font::Initialize();

	registerScripts();
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

	game->RegisterComponents();
	game->RegisterScripts();
	game->RegisterBlueprints();
	game->RegisterSceneDescriptions();

	game->Start();

	_instance.window->Loop();
}

void Engine::_Terminate() {
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

Scene *Engine::CreateScene() {
	Scene *scene = new Scene;
	_instance.scenes.push_back(scene);
	return scene;
}

Scene *Engine::CreateScene(const std::string& sceneDescription) {
	const SceneDescription& description = GetSceneDescription(sceneDescription);
	Scene *scene = new Scene(description);
	_instance.scenes.push_back(scene);
	return scene;
}

void Engine::SetActiveScene(Scene *scene) {
	_instance.active_scene = scene;
}

void Engine::SetActiveScene(const std::string& sceneDescription) {
	_instance.active_scene = CreateScene(sceneDescription);
}

Scene *Engine::GetActiveScene() {
	return _instance.active_scene;
}

void Engine::UpdateScenes(float dt) {
	for (auto scene : _instance.scenes) {
		scene->Update(dt);
	}
}

void Engine::RegisterBlueprint(Blueprint blueprint) {
	_instance.register_blueprints.insert({ blueprint.Name(), std::move(blueprint) });
}

bool Engine::HasBlueprint(const std::string& name) {
	return _instance.register_blueprints.find(name) != _instance.register_blueprints.end();
}

const Blueprint& Engine::GetBlueprint(const std::string& name) {
	auto iter = _instance.register_blueprints.find(name);
	if (iter == _instance.register_blueprints.end()) {
		throw std::runtime_error("Blueprint \"" + name + "\" does not exist.");
	}

	return iter->second;
}


void Engine::RegisterSceneDescription(SceneDescription sceneDescription) {
	_instance.register_scene_descriptions.insert({ sceneDescription.Name(), std::move(sceneDescription) });
}

bool Engine::HasSceneDescription(const std::string& name) {
	return _instance.register_scene_descriptions.find(name) != _instance.register_scene_descriptions.end();
}

const SceneDescription& Engine::GetSceneDescription(const std::string& name) {
	auto iter = _instance.register_scene_descriptions.find(name);
	if (iter == _instance.register_scene_descriptions.end()) {
		throw std::runtime_error("Scene Description \"" + name + "\" does not exist.");
	}

	return iter->second;
}

bool Engine::HasScript(const std::string& name) {
	return _instance.register_scripts.find(name) != _instance.register_scripts.end();
}

std::unique_ptr<Script> Engine::CreateScript(const std::string& name) {
	auto iter = _instance.register_scripts.find(name);
	if (iter == _instance.register_scripts.end()) {
		throw std::runtime_error("Script \"" + name = "\" does not exist.");
	}

	return iter->second();
}

SceneRenderManager& Engine::GetSceneRenderManager(Scene *scene) {
	auto iter = _instance.render_map_scene.find(scene);

	if (iter == _instance.render_map_scene.end()) {
		iter = _instance.render_map_scene.emplace(scene, scene).first;
	}

	return iter->second;
}

}
