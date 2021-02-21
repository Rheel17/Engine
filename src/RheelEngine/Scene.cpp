/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Scene.h"

namespace rheel {

Scene::Scene(Game& game) :
		_game(game),
		_registry(this) {}

Game& Scene::GetGame() {
	return _game;
}

Registry& Scene::GetRegistry() {
	return _registry;
}

const Registry& Scene::GetRegistry() const {
	return _registry;
}

Entity& Scene::AddEntity(EntityId id, const Transform& transform) {
	return _registry.AddEntity(id, transform);
}

Entity& Scene::AddEntity(const Transform& transform) {
	return _registry.AddEntity(transform);
}

void Scene::RemoveEntity(EntityId entity) {
	_registry.EraseEntity(entity);
}

void Scene::Update(float time, float dt) {
	_registry.UpdateComponents(time, dt);
}

}
