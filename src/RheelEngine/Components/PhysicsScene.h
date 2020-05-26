/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_PHYSICSSCENE_H
#define RHEELENGINE_PHYSICSSCENE_H
#include "../_common.h"

#include <btBulletDynamicsCommon.h>

#include "Rigidbody.h"

namespace rheel {

class CollisionComponent;

class RE_API PhysicsScene : public ComponentBase {
	friend class RigidBody;

private:
	struct collision_data {
		CollisionComponent* cc_0;
		CollisionComponent* cc_1;
		mutable bool has_collision;

		bool operator==(const collision_data& other) const;
	};

	struct collision_data_hash {
		std::size_t operator()(const collision_data& data) const;
	};

public:
	PhysicsScene() = default;

	void Activate() override;

	void Update() override;

	void SetGravity(vec3 gravity);

	/**
	 * Shoots a ray with a given origin and direction, and returns the closest
	 * RigidBodyComponent that intersects with the ray. A minimum and maximum
	 * distance need to be given. These values represent the search space.
	 *
	 * This function returns nullptr if there was no hit found.
	 */
	RigidBody* ShootRay(const vec3& origin, const vec3& direction, float minT, float maxT);

private:
	void AddBody_(btRigidBody* body);
	void RemoveBody_(btRigidBody* body, CollisionComponent* cc);
	void HandleCollisions_();

	PhysicsScene(const PhysicsScene& script);

	vec3 _gravity = vec3(0.0f, -9.81f, 0.0f);

	std::unordered_set<collision_data, collision_data_hash> _collisions;

	std::unique_ptr<btCollisionConfiguration> _collision_configuration;
	std::unique_ptr<btCollisionDispatcher> _dispatcher;
	std::unique_ptr<btBroadphaseInterface> _broadphase;
	std::unique_ptr<btConstraintSolver> _solver;
	std::unique_ptr<btDynamicsWorld> _world;

};

}

#endif
