/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Component.h"

namespace rheel {

Object *Component::Parent() const {
	return _parent_object;
}

}
