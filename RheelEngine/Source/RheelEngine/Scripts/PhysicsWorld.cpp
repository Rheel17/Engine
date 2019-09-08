#include "PhysicsWorld.h"

namespace rheel {

PhysicsWorld::PhysicsWorld() :
		_collisionConfiguration(new btDefaultCollisionConfiguration),
		_dispatcher(new btCollisionDispatcher(_collisionConfiguration.get())),
		_broadphase(new btDbvtBroadphase),
		_solver(new btSequentialImpulseConstraintSolver),
		_world(new btDiscreteDynamicsWorld(
				_dispatcher.get(), _broadphase.get(),
				_solver.get(), _collisionConfiguration.get())) {}


void PhysicsWorld::PreOnUpdate() {
	_world->stepSimulation(TimeDelta());
}

}
