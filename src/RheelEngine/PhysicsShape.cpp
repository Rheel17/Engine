/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PhysicsShape.h"

namespace rheel {

PhysicsShape::PhysicsShape(btCollisionShape *shape) :
		_shape(shape) {}

PhysicsShape::operator bool() const {
	return (bool) _shape;
}

btCollisionShape *PhysicsShape::_Pointer() const {
	return _shape.get();
}

PhysicsShape PhysicsShape::Box(const vec3& halfSize) {
	return PhysicsShape(new btBoxShape({ halfSize.x, halfSize.y, halfSize.z }));
}

PhysicsShape PhysicsShape::Box(float halfX, float halfY, float halfZ) {
	return PhysicsShape(new btBoxShape({ halfX, halfY, halfZ }));
}

PhysicsShape PhysicsShape::Sphere(float radius) {
	return PhysicsShape(new btSphereShape(radius));
}

PhysicsShape PhysicsShape::Capsule(float radius, float straightHeight) {
	return PhysicsShape(new btCapsuleShape(radius, straightHeight));
}

}
