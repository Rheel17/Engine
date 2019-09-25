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
class RE_API CollisionComponent : public Component {

public:
	CollisionComponent();

	~CollisionComponent() = default;

	virtual void OnCollisionStart(const CollisionComponent& other) = 0;

	virtual void OnCollisionEnd(const CollisionComponent& other) = 0;

};

}

#endif
