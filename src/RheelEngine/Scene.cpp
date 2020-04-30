/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Scene.h"

#include "Entity.h"
#include "Component.h"

namespace rheel {

Scene::Scene(Game& game) :
		_game(game),
		_root_entity(new Entity("__scene_root__", this)) {}

Scene::~Scene() {
	delete _root_entity;
}

Game& Scene::GetGame() {
	return _game;
}

Entity* Scene::AddEntity(std::string name, RigidTransform transform) {
	Entity* entity = _root_entity->AddChild(std::move(name), transform);
	_entities.push_back(entity);
	return entity;
}

std::string Scene::UniqueEntityName(const std::string& prefix) {
	return _root_entity->UniqueChildName(prefix);
}

void Scene::RemoveEntity(Entity* entity) {
	_root_entity->RemoveChild(entity);
	_entities.erase(std::find(_entities.begin(), _entities.end(), entity));
}

Entity* Scene::FindEntity(const std::string& name, bool recursive) {
	return _root_entity->FindChild(name, recursive);
}

const std::vector<Entity*>& Scene::GetEntities() const {
	return _entities;
}

void Scene::RemoveRootComponent(ComponentBase* component) {
	return _root_entity->RemoveComponent(component);
}

Camera* Scene::GetCamera(const std::string& name) {
	auto iter = _cameras.find(name);

	if (iter == _cameras.end()) {
		return nullptr;
	}

	return iter->second;
}

const std::vector<Light*>& Scene::GetLights() {
	return _lights;
}

Skybox* Scene::GetSkybox() {
	return _skybox;
}

const std::vector<ComponentInputProxy*>& Scene::GetInputComponents() const {
	return _input_components;
}

void Scene::Update(float time, float dt) {
	_root_entity->UpdateTime_(time, dt);
	_root_entity->Update();

	for (auto inputComponent : _input_components) {
		inputComponent->ResetDeltas();
	}
}

}
