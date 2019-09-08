#include "Blueprint.h"

#include <iostream>

#include "Engine.h"

namespace rheel {

Blueprint::Blueprint(std::string name) :
		_name(std::move(name)) {}

const std::string& Blueprint::Name() const {
	return _name;
}

void Blueprint::AddComponent(const std::string& componentName, std::function<void(Component&)> onLoad) {
	if (!Engine::HasComponent(componentName)) {
		throw std::runtime_error("Component not registered: \"" + componentName + "\"");
	}

	_components.push_back(std::make_pair(componentName, onLoad));
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

const std::vector<std::pair<std::string, Blueprint::_ComponentLoader>>& Blueprint::GetComponents() const {
	return _components;
}

const std::vector<std::string>& Blueprint::GetChildren() const {
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
