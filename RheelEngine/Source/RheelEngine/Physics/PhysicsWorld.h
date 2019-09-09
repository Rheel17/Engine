#ifndef PHYSICSWORLD_H_
#define PHYSICSWORLD_H_
#include "../_common.h"

#include <btBulletDynamicsCommon.h>

#include <memory>

namespace rheel {

class RE_API PhysicsWorld {

public:
	PhysicsWorld();

	void Step(float dt);

private:
	std::unique_ptr<btCollisionConfiguration> _collisionConfiguration;
	std::unique_ptr<btCollisionDispatcher> _dispatcher;
	std::unique_ptr<btBroadphaseInterface> _broadphase;
	std::unique_ptr<btConstraintSolver> _solver;
	std::unique_ptr<btDynamicsWorld> _world;

};

}

#endif
