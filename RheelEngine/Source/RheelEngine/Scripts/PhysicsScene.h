#ifndef PHYSICSSCENE_H_
#define PHYSICSSCENE_H_
#include "../_common.h"

#include <btBulletDynamicsCommon.h>

#include <memory>

#include "../Script.h"

namespace rheel {

class RE_API PhysicsScene : public Script {
	SCRIPT_INIT(PhysicsScene)

	friend class RigidBodyComponent;

public:
	PhysicsScene() = default;

	void Initialize() override;

	void PreOnUpdate() override;

	void PostOnUpdate() override;

	void SetGravity(vec3 gravity);

private:
	void _AddBody(btRigidBody *body);

	PhysicsScene(const PhysicsScene& script);

	vec3 _gravity = vec3(0.0f, -9.81f, 0.0f);

	std::unique_ptr<btCollisionConfiguration> _collisionConfiguration;
	std::unique_ptr<btCollisionDispatcher> _dispatcher;
	std::unique_ptr<btBroadphaseInterface> _broadphase;
	std::unique_ptr<btConstraintSolver> _solver;
	std::unique_ptr<btDynamicsWorld> _world;

};

}

#endif
