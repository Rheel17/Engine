/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Transform.h"

namespace rheel {

Transform::Transform(vec3 translation, quat rotation, vec3 scale) :
		translation(translation), rotation(rotation), scale(scale) {}

}

