/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Entity.h"

#include "Component.h"

namespace rheel {

Entity::Entity(std::string name, Scene *scene, RigidTransform transform) :
		name(std::move(name)), transform(std::move(transform)),
		scene(scene), parent(nullptr) {}

Entity::Entity(std::string name, Entity *parent, RigidTransform transform) :
		name(std::move(name)), transform(std::move(transform)),
		scene(parent->scene), parent(parent) {}

Entity::~Entity() {
	while (!_components.empty()) {
		RemoveComponent(_components.front());
	}
}

Entity *Entity::AddChild(std::string name, RigidTransform transform) {
	if (auto [it, inserted] = _child_names.insert(name); !inserted) {
		Log::Error() << "A child with name already exists: " << name << std::endl;
		return nullptr;
	}

	Entity *entity = new Entity(std::move(name), this, transform);
	auto ptr = std::unique_ptr<Entity>(entity);

	_children.push_back(std::move(ptr));
	return entity;
}

static inline void printChars(char *str, unsigned i) {
	str[7] = '0' + (i % 10); i /= 10;
	str[6] = '0' + (i % 10); i /= 10;
	str[5] = '0' + (i % 10); i /= 10;
	str[4] = '0' + (i % 10); i /= 10;
	str[3] = '0' + (i % 10); i /= 10;
	str[2] = '0' + (i % 10); i /= 10;
	str[1] = '0' + (i % 10); i /= 10;
	str[0] = '0' + (i % 10);
}

std::string Entity::UniqueChildName(const std::string& prefix) {
	char numberString[9] = "-0000000";

	for (unsigned i = 0; i < 10000000; i++) {
		printChars(numberString + 1, i);
		std::string tryName = prefix + numberString;

		if (std::find(_child_names.begin(), _child_names.end(), tryName) == _child_names.end()) {
			return tryName;
		}
	}

	Log::Error() << "Unable to find a unique name with prefix " << prefix << std::endl;
	return prefix;
}

void Entity::RemoveChild(Entity *entity) {
	if (entity == nullptr) {
		Log::Warning() << "Cannot remove nullptr entity" << std::endl;
		return;
	}

	if (!entity->IsDescendantOf(this)) {
		Log::Warning() << "Entity to remove is not a descendant of this" << std::endl;
		return;
	}

	if (entity->parent != this) {
		entity->parent->RemoveChild(entity);
		return;
	}

	auto iter = std::find_if(_children.begin(), _children.end(),
			[entity](const auto& ptr) { return ptr.get() == entity; });
	_children.erase(iter);
}

Entity *Entity::FindChild(const std::string& name, bool recursive) {
	auto iter = std::find_if(_children.begin(), _children.end(),
			[&name](const auto& ptr){ return ptr->name == name; });

	if (iter != _children.end()) {
		return iter->get();
	}

	if (!recursive) {
		return nullptr;
	}

	for (const auto& ptr : _children) {
		if (Entity *child = ptr->FindChild(name, true); child) {
			return child;
		}
	}

	return nullptr;
}

bool Entity::IsDescendantOf(Entity *base) {
	if (parent == base) {
		return true;
	}

	if (parent == nullptr) {
		return false;
	}

	return parent->IsDescendantOf(base);
}

void Entity::RemoveComponent(ComponentBase *component) {
	auto iter = std::find_if(_components.begin(), _components.end(),
			[component](auto ptr) { return ptr == component; });

	if (iter != _components.end()) {
		component->Deactivate();
		_components.erase(iter);
		delete component;
	}
}

mat4 Entity::CalculateAbsoluteTransformationMatrix() const {
	mat4 matrix = glm::identity<mat4>();
	const Entity *entity = this;

	while (entity != nullptr) {
		matrix = matrix * entity->transform.AsMatrix();
		entity = entity->parent;
	}

	return matrix;
}

RigidTransform Entity::CalculateAbsoluteTransform() const {
	return static_cast<RigidTransform>(Transform(CalculateAbsoluteTransformationMatrix()));
}

// This function calls the given method on all components and children
template<typename Method>
static void callAll(
		const std::vector<ComponentBase *>& components,
		const std::vector<std::unique_ptr<Entity>>& children,
		Method m) {

	for (auto component : components) {
		m(component);
	}

	for (const auto& child : children) {
		m(child);
	}
}

void Entity::Update() {
	callAll(_components, _children, [](const auto& c) { c->Update(); });
}

void Entity::Render() {
	callAll(_components, _children, [](const auto& c) { c->Render(); });
}

void Entity::_UpdateTime(float time, float dt) {
	_time = time;
	_dt = dt;

	for (auto component : _components) {
		component->_time = _time;
		component->_dt = _dt;
	}

	for (const auto& child : _children) {
		child->_UpdateTime(_time, _dt);
	}
}

}
