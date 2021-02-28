// /*
//  * Copyright (c) Levi van Rheenen. All rights reserved.
//  */
// #include "Rigidbody.h"
//
// #include "../Scene.h"
//
// #include "CollisionComponent.h"
// #include "PhysicsScene.h"
//
// namespace rheel {
//
// //static inline glm::quat bulletToGlm(const btQuaternion& q) {
// //	return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ());
// //}
// //
// //static inline btQuaternion glmToBullet(const glm::quat& q) {
// //	return btQuaternion(q.x, q.y, q.z, q.w);
// //}
//
// RigidBody::RigidBody(PhysicsShape shape, float mass, float bounciness) :
// 		_shape(std::move(shape)),
// 		_mass(mass),
// 		_bounciness(bounciness),
// 		_last_transform_update(std::make_unique<btTransform>()) {}
//
// void RigidBody::OnActivate() {
// 	if (!_shape) {
// 		throw std::runtime_error("no model set");
// 	}
//
// 	auto* physics_scene = GetEntity().GetScene().GetRootComponent<PhysicsScene>();
// 	if (!physics_scene) {
// 		return;
// 	}
//
// 	btVector3 inertia(0, 0, 0);
//
// 	if (_mass > 0) {
// 		_shape._pointer()->calculateLocalInertia(_mass, inertia);
// 	}
//
// 	_motion_state = std::make_unique<btDefaultMotionState>();
//
// 	btRigidBody::btRigidBodyConstructionInfo cinfo(_mass, _motion_state.get(), _shape._pointer(), inertia);
// 	cinfo.m_restitution = _bounciness;
//
// 	auto matrix = GetEntity().AbsoluteTransform().AsMatrix();
// 	*_last_transform_update = btTransform::getIdentity();
// 	_last_transform_update->setFromOpenGLMatrix(&matrix[0][0]);
//
// 	_body = std::make_unique<btRigidBody>(cinfo);
// 	_body->setUserPointer(this);
// 	_body->setWorldTransform(*_last_transform_update);
// 	_body->setFriction(0.5f);
//
// 	physics_scene->_add_body(_body.get());
// }
//
// void RigidBody::OnDeactivate() {
// 	auto* physics_scene = GetEntity().GetScene().GetRootComponent<PhysicsScene>();
// 	if (!physics_scene) {
// 		return;
// 	}
//
// 	physics_scene->_remove_body(_body.get(), GetEntity().GetComponent<CollisionComponent>());
// }
//
// void RigidBody::Update() {
// 	if (_body->getWorldTransform() == *_last_transform_update) {
// 		return;
// 	}
//
// 	mat4 m_prime, p_inv, c_inv;
// 	*_last_transform_update = _body->getWorldTransform();
// 	_last_transform_update->getOpenGLMatrix(&m_prime[0][0]);
//
// 	if (GetEntity().GetParent() == nullptr) {
// 		p_inv = glm::identity<mat4>();
// 	} else {
// 		p_inv = glm::inverse(GetEntity().GetParent()->AbsoluteTransform().AsMatrix());
// 	}
//
// 	c_inv = glm::inverse(GetEntity().AbsoluteTransform().AsMatrix());
//
// 	mat4 o_prime = p_inv * m_prime * c_inv;
//
// 	_transform_event_from_update = true;
// 	GetEntity().transform = Transform(o_prime);
// 	_transform_event_from_update = false;
// }
//
// void RigidBody::ApplyForce(const vec3& force) {
// 	_body->applyCentralForce({ force.x, force.y, force.z });
// }
//
// void RigidBody::ApplyImpulse(const vec3& impulse) {
// 	_body->applyCentralImpulse({ impulse.x, impulse.y, impulse.z });
// }
//
// }
