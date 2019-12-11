/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Rigidbody.h"

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
		_shape(std::move(shape)), _mass(mass), _bounciness(bounciness) {}


void RigidBody::TransformChanged() {
	if (!_transform_event_from_update) {
		auto matrix = CalculateAbsoluteTransformationMatrix();
		btTransform transform = btTransform::getIdentity();
		transform.setFromOpenGLMatrix(&matrix[0][0]);

		_body->setWorldTransform(transform);
	}
}

void RigidBody::Activate() {
	if (!_shape) {
		throw std::runtime_error("no model set");
	}

	PhysicsScene *physicsScene = GetParent()->scene->GetRootComponent<PhysicsScene>();
	if (!physicsScene) {
		return;
	}

	btVector3 inertia(0, 0, 0);

	if (_mass > 0) {
		_shape._Pointer()->calculateLocalInertia(_mass, inertia);
	}

	_motion_state = std::make_unique<btDefaultMotionState>();

	btRigidBody::btRigidBodyConstructionInfo cinfo(_mass, _motion_state.get(), _shape._Pointer(), inertia);
	cinfo.m_restitution = _bounciness;

	auto matrix = CalculateAbsoluteTransformationMatrix();
	btTransform transform = btTransform::getIdentity();
	transform.setFromOpenGLMatrix(&matrix[0][0]);

	_body = std::make_unique<btRigidBody>(cinfo);
	_body->setUserPointer(this);
	_body->setWorldTransform(transform);
	_body->setFriction(0.5f);

	physicsScene->_AddBody(_body.get());
}

void RigidBody::Update() {
	mat4 mPrime, pInv, cInv;
	_body->getWorldTransform().getOpenGLMatrix(&mPrime[0][0]);

	if (GetParent()->parent == nullptr) {
		pInv = glm::identity<mat4>();
	} else {
		pInv = glm::inverse(GetParent()->parent->CalculateAbsoluteTransformationMatrix());
	}

	cInv = glm::inverse(transform.AsMatrix());

	mat4 oPrime = pInv * mPrime * cInv;

	_transform_event_from_update = true;
	GetParent()->transform = Transform(oPrime);
	_transform_event_from_update = false;
}

void RigidBody::Deactivate() {
	PhysicsScene *physicsScene = GetParent()->scene->GetRootComponent<PhysicsScene>();
	if (!physicsScene) {
		return;
	}

	physicsScene->_RemoveBody(_body.get(), GetParent()->GetComponent<CollisionComponent>());
}

void RigidBody::ApplyForce(const vec3& force) {
	_body->applyCentralForce({ force.x, force.y, force.z });
}

void RigidBody::ApplyImpulse(const vec3& impulse) {
	_body->applyCentralImpulse({ impulse.x, impulse.y, impulse.z });
}

}
