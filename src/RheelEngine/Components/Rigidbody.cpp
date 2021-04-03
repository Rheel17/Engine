/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Rigidbody.h"

#include "../Scene.h"

#include "CollisionComponent.h"
#include "PhysicsScene.h"

namespace rheel {

//static inline glm::quat bulletToGlm(const btQuaternion& q) {
//	return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ());
//}
//
//static inline btQuaternion glmToBullet(const glm::quat& q) {
//	return btQuaternion(q.x, q.y, q.z, q.w);
//}

RigidBody::RigidBody(PhysicsShape shape, float mass, float bounciness) :
		_data({
				.last_transform_update = std::make_unique<btTransform>(),
				.shape = std::move(shape),
				.mass = mass,
				.bounciness = bounciness
		}) {}

RigidBody::RigidBody(RigidBody&& rb) noexcept :
		Component(static_cast<Component&&>(rb)),
		_data(std::move(rb._data)) {

	if (_data.body) {
		_data.body->setUserPointer(this);
	}
}

void RigidBody::OnActivate() {
	if (!_data.shape) {
		throw std::runtime_error("no model set");
	}

	auto* physics_scene = GetEntity().GetScene().GetRootComponent<PhysicsScene>();
	if (!physics_scene) {
		return;
	}

	btVector3 inertia(0, 0, 0);

	if (_data.mass > 0) {
		_data.shape._pointer()->calculateLocalInertia(_data.mass, inertia);
	}

	_data.motion_state = std::make_unique<btDefaultMotionState>();

	btRigidBody::btRigidBodyConstructionInfo cinfo(_data.mass, _data.motion_state.get(), _data.shape._pointer(), inertia);
	cinfo.m_restitution = _data.bounciness;

	auto matrix = GetEntity().AbsoluteTransform().AsMatrix();
	*_data.last_transform_update = btTransform::getIdentity();
	_data.last_transform_update->setFromOpenGLMatrix(&matrix[0][0]);

	_data.body = std::make_unique<btRigidBody>(cinfo);
	_data.body->setUserPointer(this);
	_data.body->setWorldTransform(*_data.last_transform_update);
	_data.body->setFriction(0.5f);

	physics_scene->_add_body(_data.body.get());
}

void RigidBody::OnDeactivate() {
	auto* physics_scene = GetEntity().GetScene().GetRootComponent<PhysicsScene>();
	if (!physics_scene) {
		return;
	}

	physics_scene->_remove_body(_data.body.get(), GetEntity().GetComponent<CollisionComponent>());
}

void RigidBody::Update() {
	if (_data.mass == 0) {
		// Kinematic object: update the Bullet3 body transform from the actual
		// transform.
		mat4 transform = GetEntity().AbsoluteTransform().AsMatrix();
		_data.body->getWorldTransform().setFromOpenGLMatrix(&transform[0][0]);
		return;
	}

	if (_data.body->getWorldTransform() == *_data.last_transform_update) {
		return;
	}

	// compute the parent-relative transform
	mat4 transform;
	*_data.last_transform_update = _data.body->getWorldTransform();
	_data.last_transform_update->getOpenGLMatrix(&transform[0][0]);

	if (GetEntity().GetParent() != nullptr) {
		mat4 inverse_parent = glm::inverse(GetEntity().GetParent()->AbsoluteTransform().AsMatrix());
		transform = inverse_parent * transform;
	}

	GetEntity().transform = Transform(transform);
}

void RigidBody::ApplyForce(const vec3& force) {
	_data.body->applyCentralForce({ force.x, force.y, force.z });
}

void RigidBody::ApplyImpulse(const vec3& impulse) {
	_data.body->applyCentralImpulse({ impulse.x, impulse.y, impulse.z });
}

}
