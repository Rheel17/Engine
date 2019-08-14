#include "../Engine.h"

#include "EulerCameraController.h"

namespace rheel {

void registerScripts() {
	Engine::RegisterScript<EulerCameraController>(Script::NAME_EULER_CAMERA_CONTROLLER);
}

}
