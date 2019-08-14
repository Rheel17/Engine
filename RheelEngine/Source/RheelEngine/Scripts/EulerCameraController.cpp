#include "EulerCameraController.h"

#include <cmath>

#include "../Scene.h"

namespace rheel {

void EulerCameraController::SetCamera(const std::string& camera) {
	_camera = camera;
}

void EulerCameraController::PreOnUpdate() {
	// get the camera
	CameraPtr camera = Parent().GetCamera(_camera);

	if (!camera) {
		return;
	}

	_Rotate(camera);
}

void EulerCameraController::_Rotate(CameraPtr camera) {
	vec2 mouse = InputMouseDelta();

	float yaw = glm::radians(mouse.x / 10.0f);
	float pitch = glm::radians(mouse.y / 10.0f);

	vec3 angles = camera->Rotation();
	angles.x += pitch;
	angles.y += yaw;

	if (angles.x >= 0.499f * M_PI) {
		angles.x = 0.499f * M_PI;
	}

	if (angles.x <= -0.499f * M_PI) {
		angles.x = -0.499f * M_PI;
	}

	// get the yaw in the [-pi, pi] range
	angles.y = std::fmod(std::fmod(angles.y, 2 * M_PI) + 3 * M_PI, 2 * M_PI) - M_PI;

	camera->SetRotation(angles);
}

}
