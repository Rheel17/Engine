#include "PhysicsWorld.h"

#include <btBulletDynamicsCommon.h>

namespace rheel {

void PhysicsWorld::_DeleterCollisionConfiguration::operator()(void *ptr) {
	delete static_cast<btCollisionConfiguration *>(ptr);
}

void PhysicsWorld::_DeleterDispatcher::operator()(void *ptr) {
	delete static_cast<btCollisionDispatcher *>(ptr);
}

void PhysicsWorld::_DeleterBroadphase::operator()(void *ptr) {
	delete static_cast<btBroadphaseInterface *>(ptr);
}

void PhysicsWorld::_DeleterSolver::operator()(void *ptr) {
	delete static_cast<btConstraintSolver *>(ptr);
}

void PhysicsWorld::_DeleterWorld::operator()(void *ptr) {
	delete static_cast<btDynamicsWorld *>(ptr);
}

PhysicsWorld::PhysicsWorld() {
	btCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration;
	btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface *broadphase = new btDbvtBroadphase;
	btConstraintSolver *solver = new btSequentialImpulseConstraintSolver;
	btDynamicsWorld *world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	_collision_configuration = std::unique_ptr<void, _DeleterCollisionConfiguration>(collisionConfiguration);
	_dispatcher = std::unique_ptr<void, _DeleterDispatcher>(dispatcher);
	_broadphase = std::unique_ptr<void, _DeleterBroadphase>(broadphase);
	_solver = std::unique_ptr<void, _DeleterSolver>(solver);
	_world = std::unique_ptr<void, _DeleterWorld>(world);
}

void PhysicsWorld::PreOnUpdate() {
	static_cast<btDynamicsWorld *>(_world.get())->stepSimulation(TimeDelta());
}

}
