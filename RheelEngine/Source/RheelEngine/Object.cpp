#include "Object.h"

#include <iostream>

#include "Engine.h"

namespace rheel {

Object::Object(const Blueprint& blueprint) {
	auto components = blueprint.GetComponents();
	auto children = blueprint.GetChildren();

	// add the components
	for (const auto& componentEntry : components) {
		auto component = Engine::CreateComponent(componentEntry.first);
		componentEntry.second(*component);
		component->_parent_object = this;
		_components.push_back(std::move(component));
	}

	// add the children
	for (const auto& childName : children) {
		Object& child = _children.emplace_back(Engine::GetBlueprint(childName));
		child._parent_object = this;
	}
}

Object::Object(Object&& object) noexcept :
		_parent_scene(object._parent_scene),
		_parent_object(object._parent_object),
		_alive(object._alive),
		_position(std::move(object._position)),
		_rotation(std::move(object._rotation)),
		_scale(std::move(object._scale)),
		_components(std::move(object._components)),
		_children(std::move(object._children)) {

	// re-initialize the components
	for (auto& component : _components) {
		component->_parent_object = this;
	}

	// re-initialize the children
	for (auto& child : _children) {
		child._parent_object = this;
	}
}

Object& Object::operator=(Object&& object) {
	// move fields
	_parent_scene = std::move(object._parent_scene);
	_parent_object = std::move(object._parent_object);
	_alive = std::move(object._alive);
	_position = std::move(object._position);
	_rotation = std::move(object._rotation);
	_scale = std::move(object._scale);
	_components = std::move(object._components);
	_children = std::move(object._children);

	// re-initialize the components
	for (auto& component : _components) {
		component->_parent_object = this;
	}

	// re-initialize the children
	for (auto& child : _children) {
		child._parent_object = this;
	}

	return *this;
}

ObjectPtr Object::ParentObject() {
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
			child.FireEvent(type, recursive);
		}
	}

	for (const auto& component : _components) {
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
		child._SetParentScene(scene);
	}
}

}
