#include "Blueprint.h"

#include <iostream>

#include "Engine.h"

namespace rheel {

void Blueprint::AddComponent(const std::string& componentName, std::function<void(ComponentPtr)> onLoad) {
	if (!Engine::HasComponent(componentName)) {
		throw std::runtime_error("Component not registered: \"" + componentName + "\"");
	}

	_components.push_back(componentName);
	_component_loaders.emplace(componentName, onLoad);
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

const Blueprint::_ComponentLoader& Blueprint::GetLoaderForComponent(const std::string& component) const {
	auto iter = _component_loaders.find(component);
	if (iter == _component_loaders.end()) {
		throw std::runtime_error("Component not in blueprint: \"" + component + "\"");
	}

	return iter->second;
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
