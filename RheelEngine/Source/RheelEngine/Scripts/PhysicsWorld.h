#ifndef PHYSICSWORLD_H_
#define PHYSICSWORLD_H_
#include "../_common.h"

#include <btBulletDynamicsCommon.h>

#include "../Script.h"

namespace rheel {

class RE_API PhysicsWorld : public Script {

public:
	PhysicsWorld();

	void PreOnUpdate() override;

private:
	std::shared_ptr<btCollisionConfiguration> _collisionConfiguration;
	std::shared_ptr<btCollisionDispatcher> _dispatcher;
	std::shared_ptr<btBroadphaseInterface> _broadphase;
	std::shared_ptr<btConstraintSolver> _solver;
	std::shared_ptr<btDynamicsWorld> _world;

};

}

#endif
