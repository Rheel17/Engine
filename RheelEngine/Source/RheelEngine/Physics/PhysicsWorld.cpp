#include "PhysicsWorld.h"

namespace rheel {

PhysicsWorld::PhysicsWorld() :
		_collisionConfiguration(std::make_unique<btDefaultCollisionConfiguration>()),
			_dispatcher(std::make_unique<btCollisionDispatcher>(_collisionConfiguration.get())),
			_broadphase(std::make_unique<btDbvtBroadphase>()),
			_solver(std::make_unique<btSequentialImpulseConstraintSolver>()),
			_world(std::make_unique<btDiscreteDynamicsWorld>(
					_dispatcher.get(), _broadphase.get(),
					_solver.get(), _collisionConfiguration.get())) {}

void PhysicsWorld::Step(float dt) {
	_world->stepSimulation(dt);
}

}
