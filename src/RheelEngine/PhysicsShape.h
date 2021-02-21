/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_PHYSICSSHAPE_H
#define RHEELENGINE_PHYSICSSHAPE_H
#include "_common.h"

#include <bullet/btBulletDynamicsCommon.h>

namespace rheel {

class RE_API PhysicsShape {
	friend class RigidBody;

public:
	operator bool() const;

private:
	PhysicsShape() = default;
	explicit PhysicsShape(btCollisionShape* shape);
	btCollisionShape* _pointer() const;

	std::shared_ptr<btCollisionShape> _shape;

public:
	static PhysicsShape Box(const vec3& half_size);
	static PhysicsShape Box(float half_x, float half_y, float half_z);
	static PhysicsShape Sphere(float radius);
	static PhysicsShape Capsule(float radius, float straight_height);

};

}

#endif
