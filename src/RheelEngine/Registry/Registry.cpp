/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "Registry.h"

namespace rheel {

Transform Entity::AbsoluteTransform() const {
	if (!_parent) {
		return transform;
	}

	auto parent_transform = _parent->AbsoluteTransform();
	return parent_transform * transform;
}

Entity* Entity::GetParent() {
	return _parent;
}

const Entity* Entity::GetParent() const {
	return _parent;
}

Scene& Entity::GetScene() {
	return *_registry->_scene;
}

const Scene& Entity::GetScene() const {
	return *_registry->_scene;
}

Registry::Registry(Scene* scene) :
		_scene(scene),
		_root(&AddChildEntity(nullptr, EntityId::Root(), Transform())) {}

Entity& Registry::AddEntity(EntityId id, const Transform& transform) {
	return AddChildEntity(_root, id, transform);
}

Entity& Registry::AddChildEntity(Entity* parent, EntityId id, const Transform& transform) {
	auto [ref, index] = _entities.Add(parent, this, transform);

	if (auto[iter, inserted] = _id_to_index_map.try_emplace(id._get_value(), index); !inserted) {
		throw std::runtime_error("Duplicate entity id detected (possible hash collision)");
	}

	return ref;
}

Entity& Registry::AddEntity(const Transform& transform) {
	return AddChildEntity(_root, transform);
}

Entity& Registry::AddChildEntity(Entity* parent, const Transform& transform) {
	return AddChildEntity(parent, EntityId::_generate(), transform);
}

void Registry::EraseEntity(EntityId id) {
	auto index = _entity_index(id._get_value());
	_entities.Remove(index);
}

Entity* Registry::GetEntity(EntityId id) {
	auto entity_index = _entity_index(id._get_value());
	return _entities[entity_index];
}

const Entity* Registry::GetEntity(ConstEntityId id) const {
	auto entity_index = _entity_index(id._id._get_value());
	return _entities[entity_index];
}

Entity* Registry::GetRootEntity() {
	return _root;
}

const Entity* Registry::GetRootEntity() const {
	return _root;
}

void Registry::UpdateComponents(float time, float dt) {
	// update builtin components
	for (std::size_t i = _builtin_start; i < _builtin_end; i++) {
		for (auto& component : ComponentView<Component>(_components[i])) {
			component._time = time;
			component._dt = dt;
			component.Update();
		}
	}

	// update user-defined components
	for (std::size_t i = _user_defined_start; i < _user_defined_end; i++) {
		for (auto& component : ComponentView<Component>(_components[i])) {
			component._time = time;
			component._dt = dt;
			component.Update();
		}
	}

	// reset input components
	for (auto* component : _input_components) {
		component->ResetDeltas();
	}
}

std::span<InputComponent*> Registry::GetInputComponents() {
	return _input_components;
}

}
