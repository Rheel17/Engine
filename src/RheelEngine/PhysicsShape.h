/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_PHYSICSSHAPE_H
#define RHEELENGINE_PHYSICSSHAPE_H
#include "_common.h"

#include <btBulletDynamicsCommon.h>

namespace rheel {

class RE_API PhysicsShape {
	friend class RigidBody;

public:
	operator bool() const;

private:
	PhysicsShape() = default;
	PhysicsShape(btCollisionShape* shape);
	btCollisionShape* Pointer_() const;

	std::shared_ptr<btCollisionShape> _shape;

public:
	static PhysicsShape Box(const vec3& halfSize);
	static PhysicsShape Box(float halfX, float halfY, float halfZ);
	static PhysicsShape Sphere(float radius);
	static PhysicsShape Capsule(float radius, float straightHeight);

};

}

#endif
