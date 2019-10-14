/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Entity.h"

#include "Component.h"

namespace rheel {

void Entity::_ComponentBaseDeleter::operator()(ComponentBase *ptr) const {
	delete ptr;
}

void Entity::AddComponent(ComponentBase *component) {
	_components.push_back(_ComponentBasePtr(component));
	// TODO: activate component
}

void Entity::RemoveComponent(ComponentBase *component) {
	// TODO: deactivate component
	_components.erase(std::find_if(_components.begin(), _components.end(),
			[component](const auto& ptr) { return ptr.get() == component; }));
}

}
