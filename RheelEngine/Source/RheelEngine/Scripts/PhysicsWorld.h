#ifndef PHYSICSWORLD_H_
#define PHYSICSWORLD_H_
#include "../_common.h"

#include "../Script.h"

namespace rheel {

class RE_API PhysicsWorld : public Script {
	SCRIPT_INIT(PhysicsWorld)

private:
	struct _DeleterCollisionConfiguration {	void operator()(void *ptr); };
	struct _DeleterDispatcher			  {	void operator()(void *ptr); };
	struct _DeleterBroadphase			  {	void operator()(void *ptr); };
	struct _DeleterSolver				  {	void operator()(void *ptr); };
	struct _DeleterWorld				  {	void operator()(void *ptr); };

public:
	PhysicsWorld() = default;

	void Initialize() override;

	void PreOnUpdate() override;

	void Destroy() override;

private:
	PhysicsWorld(const PhysicsWorld& script);

	std::unique_ptr<void, _DeleterCollisionConfiguration> _collision_configuration;
	std::unique_ptr<void, _DeleterDispatcher> _dispatcher;
	std::unique_ptr<void, _DeleterBroadphase> _broadphase;
	std::unique_ptr<void, _DeleterSolver> _solver;
	std::unique_ptr<void, _DeleterWorld> _world;

};

}

#endif
