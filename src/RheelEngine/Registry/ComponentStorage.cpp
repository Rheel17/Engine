/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "ComponentStorage.h"

namespace rheel {

ComponentStorage::~ComponentStorage() {
	for (auto& component : ComponentView<Component>(*this)) {
		component.~Component();
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

}