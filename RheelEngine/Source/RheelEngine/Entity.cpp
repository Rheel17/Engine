/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Entity.h"

#include "Component.h"

namespace rheel {

void Entity::_ComponentBaseDeleter::operator()(ComponentBase *ptr) const {
	delete ptr;
}

Entity::Entity(std::string name, Scene *scene, RigidTransform transform) :
		name(std::move(name)), transform(std::move(transform)),
		scene(scene), parent(nullptr) {}

Entity::Entity(std::string name, Entity *parent, RigidTransform transform) :
		name(std::move(name)), transform(std::move(transform)),
		scene(parent->scene), parent(parent) {}

Entity *Entity::AddChild(std::string name, RigidTransform transform) {
	auto entity = std::unique_ptr<Entity>(new Entity(std::move(name), this, transform));
	Entity *e = entity.get();

	_children.push_back(std::move(entity));
	return e;
}

void Entity::RemoveChild(Entity *entity) {
	auto iter = std::find_if(_children.begin(), _children.end(),
			[entity](const auto& ptr) { return ptr.get() == entity; });
	_children.erase(iter);
}

Entity *Entity::GetChild(const std::string& name) {
	auto iter = std::find_if(_children.begin(), _children.end(),
			[&name](const auto& ptr) { return ptr->name == name; });

	if (iter == _children.end()) {
		return nullptr;
	}

	return iter->get();
}

void Entity::RemoveComponent(ComponentBase *component) {
	auto iter = std::find_if(_components.begin(), _components.end(),
			[component](const auto& ptr) { return ptr.get() == component; });

	if (iter != _components.end()) {
		component->Deactivate();
		_components.erase(iter);
	}
}

}
