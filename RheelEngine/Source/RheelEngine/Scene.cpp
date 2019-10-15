/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Scene.h"

#include "Entity.h"

namespace rheel {

Scene::Scene() :
		_root_entity(new Entity("__scene_root__", this)) {}

Scene::~Scene() {
	delete _root_entity;
}

Entity *Scene::AddEntity(std::string name, RigidTransform transform) {
	return _root_entity->AddChild(std::move(name), transform);
}

void Scene::RemoveEntity(Entity *entity) {
	_root_entity->RemoveChild(entity);
}

Entity *Scene::FindEntity(const std::string& name, bool recursive) {
	return _root_entity->FindChild(name, recursive);
}

void Scene::Update() {
	_root_entity->Update();
}

}
