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

PhysicsShape PhysicsShape::Box(const vec3& half_size) {
	return PhysicsShape(new btBoxShape({ half_size.x, half_size.y, half_size.z }));
}

PhysicsShape PhysicsShape::Box(float half_x, float half_y, float half_z) {
	return PhysicsShape(new btBoxShape({ half_x, half_y, half_z }));
}

PhysicsShape PhysicsShape::Sphere(float radius) {
	return PhysicsShape(new btSphereShape(radius));
}

PhysicsShape PhysicsShape::Capsule(float radius, float straight_height) {
	return PhysicsShape(new btCapsuleShape(radius, straight_height));
}

}
