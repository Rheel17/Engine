/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "PhysicsScene.h"

#include "../Components/CollisionComponent.h"
#include "../Registry/Registry.h"

namespace rheel {

bool PhysicsScene::collision_data::operator==(const collision_data& other) const {
	return other.cc_0 == cc_0 && other.cc_1 == cc_1;
}

std::size_t PhysicsScene::collision_data_hash::operator()(const collision_data& data) const {
	return std::size_t(data.cc_0) * std::size_t(data.cc_1) * 0x4e911fa1;
}

void PhysicsScene::OnActivate() {
	_collision_configuration = std::make_unique<btDefaultCollisionConfiguration>();
	_dispatcher = std::make_unique<btCollisionDispatcher>(_collision_configuration.get());
	_broadphase = std::make_unique<btDbvtBroadphase>();
	_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
	_world = std::make_unique<btDiscreteDynamicsWorld>(_dispatcher.get(), _broadphase.get(), _solver.get(), _collision_configuration.get());

	_world->setGravity({ _gravity.x, _gravity.y, _gravity.z });
}

void PhysicsScene::Update() {
	if (_world) {
		_world->stepSimulation(dt);
		_handle_collisions();
	}
}

void PhysicsScene::SetGravity(vec3 gravity) {
	_gravity = gravity;

	if (_world) {
		_world->setGravity({ _gravity.x, _gravity.y, _gravity.z });
	}
}

RigidBody* PhysicsScene::ShootRay(const vec3& origin, const vec3& direction, float min_t, float max_t) {
	// calculate the from and to positions
	vec3 dir = glm::normalize(direction);

	btVector3 from = {
			origin.x + min_t * dir.x,
			origin.y + min_t * dir.y,
			origin.z + min_t * dir.z
	};

	btVector3 to = {
			origin.x + max_t * dir.x,
			origin.y + max_t * dir.y,
			origin.z + max_t * dir.z
	};

	btCollisionWorld::ClosestRayResultCallback callback(from, to);
	_world->rayTest(from, to, callback);

	if (!callback.hasHit()) {
		return nullptr;
	}

	return static_cast<RigidBody*>(callback.m_collisionObject->getUserPointer());
}

void PhysicsScene::_add_body(btRigidBody* body) {
	_world->addRigidBody(body);
}

void PhysicsScene::_remove_body(btRigidBody* body, CollisionComponent* cc) {
	// handle current collisions
	std::vector<collision_data> to_remove;

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
			to_remove.push_back(collision);
		}
	}

	for (const collision_data& data : to_remove) {
		_collisions.erase(data);
	}

	// remove from the world
	_world->removeRigidBody(body);
}

void PhysicsScene::_handle_collisions() {
	for (int i = 0; i < _world->getDispatcher()->getNumManifolds(); i++) {
		auto* manifold = _world->getDispatcher()->getManifoldByIndexInternal(i);

		if (manifold->getNumContacts() == 0) {
			continue;
		}

		auto* body_0 = static_cast<RigidBody*>(manifold->getBody0()->getUserPointer());
		auto* body_1 = static_cast<RigidBody*>(manifold->getBody1()->getUserPointer());

		auto* cc_0 = body_0->GetEntity().GetComponent<CollisionComponent>();
		if (!cc_0) {
			continue;
		}

		auto* cc_1 = body_1->GetEntity().GetComponent<CollisionComponent>();
		if (!cc_1) {
			continue;
		}

		auto iter = _collisions.insert({ cc_0, cc_1, false });
		iter.first->has_collision = true;

		if (iter.second) {
			cc_0->OnCollisionStart(*cc_1);
			cc_1->OnCollisionStart(*cc_0);
		}
	}

	std::vector<collision_data> to_remove;

	for (const collision_data& data : _collisions) {
		if (!data.has_collision) {
			to_remove.push_back(data);
			data.cc_0->OnCollisionEnd(*data.cc_1);
			data.cc_1->OnCollisionEnd(*data.cc_0);
		} else {
			data.has_collision = false;
		}
	}

	for (const collision_data& data : to_remove) {
		_collisions.erase(data);
	}
}

}
