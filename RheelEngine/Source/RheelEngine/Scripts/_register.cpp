#include "../Engine.h"

#include "EulerCameraController.h"
#include "PhysicsWorld.h"

namespace rheel {

void registerScripts() {
	Engine::RegisterScript<EulerCameraController>(Script::NAME_EULER_CAMERA_CONTROLLER);
	Engine::RegisterScript<PhysicsWorld>(Script::NAME_PHYSICS_WORLD);
}

}
