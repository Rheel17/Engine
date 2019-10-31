/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "RigidbodyComponent.h"

#include "CollisionComponent.h"
#include "PhysicsScene.h"

namespace rheel {

static inline glm::quat bulletToGlm(const btQuaternion& q) {
	return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ());
}

static inline btQuaternion glmToBullet(const glm::quat& q) {
	return btQuaternion(q.x, q.y, q.z, q.w);
}

RigidBodyComponent::RigidBodyComponent(PhysicsShape shape, float mass, float bounciness) :
		_shape(std::move(shape)), _mass(mass), _bounciness(bounciness) {}

void RigidBodyComponent::Activate() {
	if (!_shape) {
		throw std::runtime_error("no model set");
	}

	PhysicsScene *physicsScene = GetParent()->scene->GetRootComponent<PhysicsScene>();
	if (!physicsScene) {
		return;
	}

//	const vec3& position = Parent()->Position();
//	const quat& rotation = Parent()->Rotation();
//
	btVector3 inertia(0, 0, 0);

	if (_mass > 0) {
		_shape._Pointer()->calculateLocalInertia(_mass, inertia);
	}

	_motion_state = std::make_unique<btDefaultMotionState>();

	btRigidBody::btRigidBodyConstructionInfo cinfo(_mass, _motion_state.get(), _shape._Pointer(), inertia);
	cinfo.m_restitution = _bounciness;

	// TODO: do transform math.
//	btTransform transform = btTransform::getIdentity();
//	transform.setOrigin({ position.x, position.y, position.z });
//	transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
//
//	_body = std::make_unique<btRigidBody>(cinfo);
//	_body->setUserPointer(this);
//	_body->setWorldTransform(transform);
//	_body->setFriction(0.5f);
//
//	physicsScene->_AddBody(_body.get());
}

void RigidBodyComponent::Update() {
//	btVector3 position = _body->getWorldTransform().getOrigin();
//	btQuaternion rotation = _body->getWorldTransform().getRotation();
//
//	Parent()->SetPosition(position.x(), position.y(), position.z());
//	Parent()->SetRotation(quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ()));
}

void RigidBodyComponent::Deactivate() {
//	PhysicsScene *physicsScene = Parent()->ParentScene()->GetScript<PhysicsScene>();
//	if (!physicsScene) {
//		return;
//	}
//
//	physicsScene->_RemoveBody(_body.get(), Parent()->GetComponent<CollisionComponent>());
}

void RigidBodyComponent::ApplyForce(const vec3& force) {
	_body->applyCentralForce({ force.x, force.y, force.z });
}

void RigidBodyComponent::ApplyImpulse(const vec3& impulse) {
	_body->applyCentralImpulse({ impulse.x, impulse.y, impulse.z });
}

}
