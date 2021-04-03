/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "ComponentStorage.h"

#include "Registry.h"

namespace rheel {

ComponentStorage::~ComponentStorage() {
	// Simulate removing all components in this storage from their entities.
	// This can be done simpler than the 'normal' way, because we don't need to
	// keep the storage continguous.
	// => This means that during deactivation/destruction of a component, we
	//    cannot use other components from the same type, or interact with them
	//    in any way.

	// We also know that this destructor will only be called when the entire
	// registry is being deleted. That removes some additional housekeeping as
	// well.

	for (auto& component : ComponentView<Component>(*this)) {
		// Store reference from entity to this component, since we're going to
		// destruct it before we use those variables.
		auto* entity = component._entity;
		auto index_in_entity = component._index_in_entity;

		// Deactivate
		component.OnDeactivate();

		// Call destructor
		component.~Component();

		// Remove reference from entity. Leaving nullptr is fine here, since the
		// entity will be destroyed soon. We're not worried about the storage
		// space in its _components field any more.
		entity->_components[index_in_entity] = nullptr;
	}

	free(_storage); // NOLINT (allocated by malloc())
	_storage = nullptr;
}

ComponentStorage::ComponentStorage(ComponentStorage&& cs) noexcept :
		_storage(cs._storage),
		_byte_capacity(cs._byte_capacity),
		_size(cs._size) {

	cs._storage = nullptr;
	cs._byte_capacity = 0;
	cs._size = 0;
}

Component& ComponentStorage::operator[](std::size_t idx) {
	return *(ComponentView<Component>(*this).begin() + idx);
}

const Component& ComponentStorage::operator[](std::size_t idx) const {
	return *(ComponentView<const Component>(*this).begin() + idx);
}

Component** ComponentStorage::_component_pp(Entity* entity, std::size_t index_in_entity) {
	return &(entity->_components[index_in_entity]);
}

}
