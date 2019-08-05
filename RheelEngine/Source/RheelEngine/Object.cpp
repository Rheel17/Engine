#include "Object.h"

#include <iostream>

#include "Engine.h"

namespace rheel {

Object::Object(const Blueprint& blueprint) {
	auto components = blueprint.Components();
	auto children = blueprint.Children();

	// add the components
	for (const auto& componentName : components) {
		ComponentPtr component = Engine::CreateComponent(componentName);
		blueprint.GetLoaderForComponent(componentName)(component);
		_components.push_back(component);
	}

	// add the children
	for (const auto& childName : children) {
		Object *child = new Object(Engine::GetBlueprint(childName));
		_children.push_back(child);
		child->_parent_object = this;
	}

	// initialize the components
	for (auto component : _components) {
		component->_parent_object = this;
	}
}

Object *Object::ParentObject() {
	return _parent_object;
}

Scene *Object::ParentScene() {
	return _parent_scene;
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

void Object::SetScale(const vec3& scale) {
	SetScale(scale.x, scale.y, scale.z);
}

void Object::SetScale(float x, float y, float z) {
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
}

void Object::SetScale(float scale) {
	_scale.x = scale;
	_scale.y = scale;
	_scale.z = scale;
}

const vec3& Object::Scale() const {
	return _scale;
}

void Object::FireEvent(EventType type, bool recursive) {
	if (recursive) {
		for (auto& child : _children) {
			child->FireEvent(type, recursive);
		}
	}

	for (auto component : _components) {
		switch (type) {
			case ON_ADD:             component->OnAdd();             break;
			case ON_REMOVE:          component->OnRemove();          break;
			case ON_UPDATE:          component->OnUpdate();          break;
			case ON_UPDATE_RENDERER: component->OnUpdateRenderers(); break;
			case ON_RENDER:          component->OnRender();          break;
		}
	}
}

void Object::_SetParentScene(Scene *scene) {
	_parent_scene = scene;

	for (auto& child : _children) {
		child->_SetParentScene(scene);
	}
}

}
