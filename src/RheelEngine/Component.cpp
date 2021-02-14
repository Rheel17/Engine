/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Component.h"

namespace rheel {

Component::Component(Component&& c) noexcept :
		_entity(c._entity),
		_index(c._index),
		_size(c._size),
		_index_in_entity(c._index_in_entity) {}

Component& Component::operator=(Component&& c) noexcept {
	_entity = c._entity;
	_index = c._index;
	_size = c._size;
	_index_in_entity = c._index_in_entity;
	return *this;
}

Entity& Component::GetEntity() {
	return *_entity;
}

const Entity& Component::GetEntity() const {
	return *_entity;
}

}
