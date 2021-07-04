/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_ENTITYIMPL_INC
#define ENGINE_ENTITYIMPL_INC

#ifndef ENGINE_REGISTRY_H
#error "Do not include directly, include via Registry.h"
#endif

namespace rheel {

template<ComponentBaseClass C>
C* Entity::GetComponent() {
	for (auto* component : _components) {
		if (auto* c = dynamic_cast<C*>(component)) {
			return c;
		}
	}

	return nullptr;
}

template<ComponentBaseClass C>
const C* Entity::GetComponent() const {
	for (auto* component : _components) {
		if (const auto* c = dynamic_cast<const C*>(component)) {
			return c;
		}
	}

	return nullptr;
}

template<typename C, typename... Args>
C& Entity::AddComponent(Args&&... args) {
	return _registry->AddComponent<C>(this, std::forward<Args>(args)...);
}

template<typename C>
bool Entity::RemoveComponent() {
	if (auto* instance = GetComponent<C>()) {
		_registry->RemoveComponent<C>(instance->_index);
		return true;
	}

	return false;
}

}

#endif
