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

	std::cout << "[" << std::endl;

	for (int i = 0; i < _world->getDispatcher()->getNumManifolds(); i++) {
		auto manifold = _world->getDispatcher()->getManifoldByIndexInternal(i);
		std::cout << " " << manifold << std::endl;
	}

	std::cout << "]" << std::endl;
}

void PhysicsScene::SetGravity(vec3 gravity) {
	_gravity = std::move(gravity);

	if (_world) {
		_world->setGravity({ _gravity.x, _gravity.y, _gravity.z });
	}
}

RigidBodyComponent *PhysicsScene::ShootRay(const vec3& origin, const vec3& direction, float minT, float maxT) {
	// calculate the from and to positions
	vec3 dir = glm::normalize(direction);

	btVector3 from = {
			origin.x + minT * dir.x,
			origin.y + minT * dir.y,
			origin.z + minT * dir.z
	};

	btVector3 to = {
			origin.x + maxT * dir.x,
			origin.y + maxT * dir.y,
			origin.z + maxT * dir.z
	};

	btCollisionWorld::ClosestRayResultCallback callback(from, to);
	_world->rayTest(from, to, callback);

	if (!callback.hasHit()) {
		return nullptr;
	}

	return static_cast<RigidBodyComponent *>(callback.m_collisionObject->getUserPointer());
}

void PhysicsScene::_AddBody(btRigidBody *body) {
	_world->addRigidBody(body);
}

}
