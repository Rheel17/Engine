/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef PHYSICSSHAPE_H_
#define PHYSICSSHAPE_H_
#include "_common.h"

#include <btBulletDynamicsCommon.h>

#include <memory>

namespace rheel {

class RE_API PhysicsShape {
	friend class RigidBodyComponent;

public:
	operator bool() const;

private:
	PhysicsShape() = default;
	PhysicsShape(btCollisionShape *shape);
	btCollisionShape *_Pointer() const;

	std::shared_ptr<btCollisionShape> _shape;

public:
	static PhysicsShape Box(const vec3& halfSize);
	static PhysicsShape Box(float halfX, float halfY, float halfZ);
	static PhysicsShape Sphere(float radius);

};

}

#endif
