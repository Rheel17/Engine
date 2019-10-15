/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Object.h"

#include "Engine.h"

namespace rheel {

Object::Object(const Blueprint& blueprint) :
		_blueprint_name(blueprint.Name()) {

	const auto& components = blueprint._Components();
	const auto& children = blueprint._Children();

	// add the components
	for (const auto& component : components) {
		std::unique_ptr<ComponentOld> componentCopy(component->__CloneHeap());
		componentCopy->_parent_object = this;
		_components.push_back(std::move(componentCopy));
	}

	// add the children
	for (const auto& childName : children) {
		auto& child = _children.emplace_back(std::make_unique<Object>(Engine::GetBlueprint(childName)));
		child->_parent_object = this;
	}
}

Object::Object(Object&& object) noexcept :
		_blueprint_name(std::move(object._blueprint_name)),
		_parent_scene(object._parent_scene),
		_parent_object(object._parent_object),
		_alive(object._alive),
		_position(std::move(object._position)),
		_rotation(std::move(object._rotation)),
		_components(std::move(object._components)),
		_children(std::move(object._children)) {

	// re-initialize the components
	for (auto& component : _components) {
		component->_parent_object = this;
	}

	// re-initialize the children
	for (auto& child : _children) {
		child->_parent_object = this;
	}
}

Object& Object::operator=(Object&& object) {
	// move fields
	_blueprint_name = std::move(object._blueprint_name);
	_parent_scene = std::move(object._parent_scene);
	_parent_object = std::move(object._parent_object);
	_alive = std::move(object._alive);
	_position = std::move(object._position);
	_rotation = std::move(object._rotation);
	_components = std::move(object._components);
	_children = std::move(object._children);

	// re-initialize the components
	for (auto& component : _components) {
		component->_parent_object = this;
	}

	// re-initialize the children
	for (auto& child : _children) {
		child->_parent_object = this;
	}

	return *this;
}

Object& Object::ParentObject() {
	return *_parent_object;
}

SceneOld *Object::ParentScene() {
	return _parent_scene;
}

const std::string& Object::BlueprintName() const {
	return _blueprint_name;
}

void Object::SetPosition(const vec3& position) {
	SetPosition(position.x, position.y, position.z);
}

void Object::SetPosition(float x, float y, float z) {
	_position.x = x;
	_position.y = y;
	_position.z = z;
}

void Object::Move(const vec3& position) {
	Move(position.x, position.y, position.z);
}

void Object::Move(float x, float y, float z) {
	_position.x += x;
	_position.y += y;
	_position.z += z;
}

const vec3& Object::Position() const {
	return _position;
}

void Object::SetRotation(const quat& rotation) {
	_rotation = rotation;
}

void Object::Rotate(const quat& rotation) {
	_rotation *= rotation;
}

const quat& Object::Rotation() const {
	return _rotation;
}

void Object::FireEvent(EventType type, bool recursive) {
	if (recursive) {
		for (auto& child : _children) {
			child->FireEvent(type, recursive);
		}
	}

	std::vector<ComponentOld *> components;
	for (const auto& component : _components) {
		components.push_back(component.get());
	}

	for (auto component : components) {
		switch (type) {
			case ON_ADD:             component->OnAdd();             break;
			case ON_REMOVE:          component->OnRemove();          break;
			case ON_UPDATE:          component->OnUpdate(_dt);       break;
			case ON_UPDATE_RENDERER: component->OnUpdateRenderers(); break;
		}
	}
}

void Object::RemoveComponent(ComponentOld& component) {
	ComponentOld *componentPtr = &component;

	_components.erase(std::find_if(_components.begin(), _components.end(), [componentPtr](const auto& ptr) {
		return ptr.get() == componentPtr;
	}));
}

void Object::_SetParentScene(SceneOld *scene) {
	_parent_scene = scene;

	for (auto& child : _children) {
		child->_SetParentScene(scene);
	}
}

}
