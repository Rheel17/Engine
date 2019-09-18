#include "PhysicsScene.h"

namespace rheel {

PhysicsScene::PhysicsScene(const PhysicsScene& script) :
		_gravity(script._gravity) {}

void PhysicsScene::Initialize() {
	_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
	_dispatcher = std::make_unique<btCollisionDispatcher>(_collisionConfiguration.get());
	_broadphase = std::make_unique<btDbvtBroadphase>();
	_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
	_world = std::make_unique<btDiscreteDynamicsWorld>(
				_dispatcher.get(), _broadphase.get(),
				_solver.get(), _collisionConfiguration.get());

	_world->setGravity({ _gravity.x, _gravity.y, _gravity.z });
}

void PhysicsScene::PreOnUpdate() {
	_world->stepSimulation(TimeDelta());
}

void PhysicsScene::PostOnUpdate() {}

void PhysicsScene::SetGravity(vec3 gravity) {
	_gravity = std::move(gravity);

	if (_world) {
		_world->setGravity({ _gravity.x, _gravity.y, _gravity.z });
	}
}

void PhysicsScene::_AddBody(btRigidBody *body) {
	_world->addRigidBody(body);
}

}
