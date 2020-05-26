/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PhysicsScene.h"

#include "../Components/CollisionComponent.h"

namespace rheel {

bool PhysicsScene::collision_data::operator==(const collision_data& other) const {
	return other.cc_0 == cc_0 && other.cc_1 == cc_1;
}

std::size_t PhysicsScene::collision_data_hash::operator()(const collision_data& data) const {
	return std::size_t(data.cc_0) * std::size_t(data.cc_1) * 0x4e911fa1;
}

PhysicsScene::PhysicsScene(const PhysicsScene& script) :
		_gravity(script._gravity) {}

void PhysicsScene::Activate() {
	_collision_configuration = std::make_unique<btDefaultCollisionConfiguration>();
	_dispatcher = std::make_unique<btCollisionDispatcher>(_collision_configuration.get());
	_broadphase = std::make_unique<btDbvtBroadphase>();
	_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
	_world = std::make_unique<btDiscreteDynamicsWorld>(_dispatcher.get(), _broadphase.get(), _solver.get(), _collision_configuration.get());

	_world->setGravity({ _gravity.x, _gravity.y, _gravity.z });
}

void PhysicsScene::Update() {
	_world->stepSimulation(GetTimeDelta());
	HandleCollisions_();
}

void PhysicsScene::SetGravity(vec3 gravity) {
	_gravity = std::move(gravity);

	if (_world) {
		_world->setGravity({ _gravity.x, _gravity.y, _gravity.z });
	}
}

RigidBody* PhysicsScene::ShootRay(const vec3& origin, const vec3& direction, float minT, float maxT) {
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

	return static_cast<RigidBody*>(callback.m_collisionObject->getUserPointer());
}

void PhysicsScene::AddBody_(btRigidBody* body) {
	_world->addRigidBody(body);
}

void PhysicsScene::RemoveBody_(btRigidBody* body, CollisionComponent* cc) {
	// handle current collisions
	std::vector<collision_data> toRemove;

	for (const auto& collision : _collisions) {
		bool remove = true;

		if (collision.cc_0 == cc) {
			collision.cc_1->OnCollisionEnd(*cc);
		} else if (collision.cc_1 == cc) {
			collision.cc_0->OnCollisionEnd(*cc);
		} else {
			remove = false;
		}

		if (remove) {
			toRemove.push_back(collision);
		}
	}

	for (const collision_data& data : toRemove) {
		_collisions.erase(data);
	}

	// remove from the world
	_world->removeRigidBody(body);
}

void PhysicsScene::HandleCollisions_() {
	for (int i = 0; i < _world->getDispatcher()->getNumManifolds(); i++) {
		auto manifold = _world->getDispatcher()->getManifoldByIndexInternal(i);

		if (manifold->getNumContacts() == 0) {
			continue;
		}

		auto body0 = static_cast<RigidBody*>(manifold->getBody0()->getUserPointer());
		auto body1 = static_cast<RigidBody*>(manifold->getBody1()->getUserPointer());

		// TODO: notify all CollisionComponents?
		auto cc0 = body0->GetParent()->GetComponent<CollisionComponent>();
		if (!cc0) {
			continue;
		}

		auto cc1 = body1->GetParent()->GetComponent<CollisionComponent>();
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

	std::vector<collision_data> toRemove;

	for (const collision_data& data : _collisions) {
		if (!data.has_collision) {
			toRemove.push_back(data);
			data.cc_0->OnCollisionEnd(*data.cc_1);
			data.cc_1->OnCollisionEnd(*data.cc_0);
		} else {
			data.has_collision = false;
		}
	}

	for (const collision_data& data : toRemove) {
		_collisions.erase(data);
	}
}

}
