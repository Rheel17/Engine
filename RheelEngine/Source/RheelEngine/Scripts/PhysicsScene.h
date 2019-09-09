#ifndef PHYSICSSCENE_H_
#define PHYSICSSCENE_H_
#include "../_common.h"

#include <memory>

#include "../Script.h"
#include "../Physics/PhysicsWorld.h"

namespace rheel {

class RE_API PhysicsScene : public Script {
	SCRIPT_INIT(PhysicsScene)

public:
	PhysicsScene() = default;

	void Initialize() override;

	void PreOnUpdate() override;

private:
	PhysicsScene(const PhysicsScene& script);

private:
	std::unique_ptr<PhysicsWorld> _world;

};

}

#endif
