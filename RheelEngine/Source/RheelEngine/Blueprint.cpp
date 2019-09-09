#include "Blueprint.h"

#include <iostream>

#include "Engine.h"

namespace rheel {

Blueprint::Blueprint(std::string name) :
		_name(std::move(name)) {}

Blueprint::Blueprint(const Blueprint& blueprint) :
		_name(blueprint._name),
		_child_blueprints(blueprint._child_blueprints) {

	for (const auto& component : blueprint._components) {
		_components.emplace_back(component->__CloneHeap());
	}
}

const std::string& Blueprint::Name() const {
	return _name;
}

void Blueprint::AddChild(const std::string& blueprintName) {
	if (_name == blueprintName) {
		throw std::runtime_error("A blueprint cannot be child of itself");
	}

	if (!Engine::HasBlueprint(blueprintName)) {
		throw std::runtime_error("Blueprint not registered: \"" + blueprintName + "\"");
	}

	if (Engine::GetBlueprint(blueprintName)._HasChild(_name)) {
		throw std::runtime_error("Cyclic blueprint references not allowed");
	}

	_child_blueprints.push_back(blueprintName);
}

const std::vector<std::unique_ptr<Component>>& Blueprint::_Components() const {
	return _components;
}

const std::vector<std::string>& Blueprint::_Children() const {
	return _child_blueprints;
}

bool Blueprint::_HasChild(const std::string& name) const {
	// first: check the children
	for (const auto& child : _child_blueprints) {
		if (child == name) {
			return true;
		}
	}

	// second: recursively check the children
	for (const auto& child : _child_blueprints) {
		if (Engine::GetBlueprint(child)._HasChild(name)) {
			return true;
		}
	}

	// all failed: so the given blueprint is not a child.
	return false;
}

}
