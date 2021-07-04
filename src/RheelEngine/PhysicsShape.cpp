/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PhysicsShape.h"

namespace rheel {

PhysicsShape::PhysicsShape(btCollisionShape* shape) :
		_shape(shape) {}

PhysicsShape::operator bool() const {
	return (bool) _shape;
}

btCollisionShape* PhysicsShape::_pointer() const {
	return _shape.get();
}

PhysicsShape PhysicsShape::Box(const vec3& size) {
	return Box(size.x, size.y, size.z);
}

PhysicsShape PhysicsShape::Box(float x, float y, float z) {
	return PhysicsShape(new btBoxShape({ x * 0.5f, y * 0.5f, z * 0.5f }));
}

PhysicsShape PhysicsShape::Sphere(float radius) {
	return PhysicsShape(new btSphereShape(radius));
}

PhysicsShape PhysicsShape::Capsule(float radius, float straight_height) {
	return PhysicsShape(new btCapsuleShape(radius, straight_height));
}

}
