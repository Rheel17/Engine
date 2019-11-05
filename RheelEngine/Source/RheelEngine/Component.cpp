/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Component.h"

namespace rheel {

Entity *ComponentBase::GetParent() {
	return _entity;
}

const Entity *ComponentBase::GetParent() const {
	return _entity;
}

}


