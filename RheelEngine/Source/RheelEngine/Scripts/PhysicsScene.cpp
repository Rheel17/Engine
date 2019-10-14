/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "PhysicsScene.h"

#include "../Components/CollisionComponent.h"
#include "../Object.h"

namespace rheel {

bool PhysicsScene::_CollisionData::operator==(const _CollisionData& other) const {
	return other.cc0 == cc0 && other.cc1 == cc1;
}

constexpr std::size_t PhysicsScene::_CollisionDataHash::operator()(const _CollisionData& data) const {
	return std::size_t(data.cc0) * std::size_t(data.cc1) * 0x4e911fa1;
}

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
	_HandleCollisions();
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

void PhysicsScene::_RemoveBody(btRigidBody *body, CollisionComponent *cc) {
	// handle current collisions
	std::vector<_CollisionData> toRemove;

	for (const auto& collision : _collisions) {
		bool remove = true;

		if (collision.cc0 == cc) {
			collision.cc1->OnCollisionEnd(*cc);
		} else if (collision.cc1 == cc) {
			collision.cc0->OnCollisionEnd(*cc);
		} else {
			remove = false;
		}

		if (remove) {
			toRemove.push_back(collision);
		}
	}

	for (const _CollisionData& data : toRemove) {
		_collisions.erase(data);
	}

	// remove from the world
	_world->removeRigidBody(body);
}

void PhysicsScene::_HandleCollisions() {
	for (int i = 0; i < _world->getDispatcher()->getNumManifolds(); i++) {
		auto manifold = _world->getDispatcher()->getManifoldByIndexInternal(i);

		if (manifold->getNumContacts() == 0) {
			continue;
		}

		auto body0 = static_cast<RigidBodyComponent *>(manifold->getBody0()->getUserPointer());
		auto body1 = static_cast<RigidBodyComponent *>(manifold->getBody1()->getUserPointer());

		auto cc0 = body0->Parent()->GetComponent<CollisionComponent>();
		if (!cc0) {
			continue;
		}

		auto cc1 = body1->Parent()->GetComponent<CollisionComponent>();
		if (!cc1) {
			continue;
		}

		auto iter = _collisions.insert({ cc0, cc1, false });
		iter.first->has_collision = true;

		if (iter.second) {
			cc0->OnCollisionStart(*cc1);
			cc1->OnCollisionStart(*cc0);
		}
	}

	std::vector<_CollisionData> toRemove;

	for (const _CollisionData& data : _collisions) {
		if (!data.has_collision) {
			toRemove.push_back(data);
			data.cc0->OnCollisionEnd(*data.cc1);
			data.cc1->OnCollisionEnd(*data.cc0);
		} else {
			data.has_collision = false;
		}
	}

	for (const _CollisionData& data : toRemove) {
		_collisions.erase(data);
	}
}

}
