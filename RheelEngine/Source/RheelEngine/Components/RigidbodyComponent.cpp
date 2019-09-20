#include "RigidBodyComponent.h"

#include "../Scene.h"
#include "../Scripts/PhysicsScene.h"
#include "../Object.h"

namespace rheel {

RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent& component) :
		_shape(component._shape),
		_mass(component._mass),
		_bounciness(component._bounciness) {}

glm::quat bulletToGlm(const btQuaternion& q) {
	return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ());
}

btQuaternion glmToBullet(const glm::quat& q) {
	return btQuaternion(q.x, q.y, q.z, q.w);
}

void RigidBodyComponent::OnAdd() {
	if (!_shape) {
		throw std::runtime_error("no model set");
	}

	PhysicsScene *physicsScene = Parent().ParentScene()->GetScript<PhysicsScene>();
	if (!physicsScene) {
		return;
	}

	const vec3& position = Parent().Position();
	const quat& rotation = Parent().Rotation();

	btVector3 inertia(0, 0, 0);

	if (_mass > 0) {
		_shape._Pointer()->calculateLocalInertia(_mass, inertia);
	}

	_motion_state = std::make_unique<btDefaultMotionState>();

	btRigidBody::btRigidBodyConstructionInfo cinfo(_mass, _motion_state.get(), _shape._Pointer(), inertia);
	cinfo.m_restitution = _bounciness;

	btTransform transform = btTransform::getIdentity();
	transform.setOrigin({ position.x, position.y, position.z });
	transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

	_body = std::make_unique<btRigidBody>(cinfo);
	_body->setWorldTransform(transform);
	_body->setFriction(0.5f);

	physicsScene->_AddBody(_body.get());
}

void RigidBodyComponent::OnUpdate() {
	btVector3 position = _body->getWorldTransform().getOrigin();
	btQuaternion rotation = _body->getWorldTransform().getRotation();

	Parent().SetPosition(position.x(), position.y(), position.z());
	Parent().SetRotation(quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ()));
}

void RigidBodyComponent::OnRemove() {
	// TODO implement
}

void RigidBodyComponent::SetShape(PhysicsShape shape) {
	if (_body) {
		throw std::runtime_error("Cannot change shape while active");
	}

	_shape = std::move(shape);
}

void RigidBodyComponent::SetMass(float mass) {
	if (_body) {
		throw std::runtime_error("Cannot change mass while active");
	}

	_mass = mass;
}

void RigidBodyComponent::SetBounciness(float bounciness) {
	if (_body) {
		throw std::runtime_error("Cannot change bounciness while active");
	}

	_bounciness = bounciness;
}

void RigidBodyComponent::ApplyForce(const vec3& force) {
	_body->applyCentralForce({ force.x, force.y, force.z });
}

void RigidBodyComponent::ApplyImpulse(const vec3& impulse) {
	_body->applyCentralImpulse({ impulse.x, impulse.y, impulse.z });
}

}
