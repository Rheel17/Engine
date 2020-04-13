/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef COLLISIONCOMPONENT_H_
#define COLLISIONCOMPONENT_H_
#include "../_common.h"

#include "../Component.h"

namespace rheel {

/**
 * Handles collision events between two objects. In order for this component to
 * work, a RigidBodyComponent must be added before this component. Only
 * collisions with other objects having a CollisionComponent will be detected.
 */
class RE_API CollisionComponent : public ComponentBase {

public:
	CollisionComponent() = default;

	~CollisionComponent() override = default;

	virtual void OnCollisionStart(CollisionComponent& other) {}

	virtual void OnCollisionEnd(CollisionComponent& other) {}

};

}

#endif
