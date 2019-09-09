#include "EulerCameraController.h"

#include "../Scene.h"

namespace rheel {

void EulerCameraController::SetCamera(const std::string& camera) {
	_camera = camera;
}

void EulerCameraController::SetAngularVelocity(float velocity) {
	SetAngularVelocity(velocity, velocity);
}

void EulerCameraController::SetAngularVelocity(float yawVelocity, float pitchVelocity) {
	_velocity_yaw = yawVelocity;
	_velocity_pitch = pitchVelocity;
}

void EulerCameraController::SetVelocity(float velocity) {
	SetVelocity(velocity, velocity);
}

void EulerCameraController::SetVelocity(float forwardsBackwardsVelocity, float sideVelocity) {
	_velocity_z = forwardsBackwardsVelocity;
	_velocity_x = sideVelocity;
}


void EulerCameraController::PreOnUpdate() {
	// get the camera
	Camera *camera = Parent().GetCamera(_camera);

	if (!camera) {
		return;
	}

	_Rotate(*camera, TimeDelta());
	_Move(*camera, TimeDelta());
}

void EulerCameraController::_Rotate(Camera& camera, float dt) {
	vec2 mouse = InputMouseDelta();

	float yaw = glm::radians(mouse.x * _velocity_yaw / 100.0f);
	float pitch = glm::radians(mouse.y * _velocity_pitch / 100.0f);

	vec3 angles = camera.Rotation();
	angles.x -= pitch;
	angles.y -= yaw;

	// prevent more than 90 degrees look up/down
	if (angles.x >= 0.499f * M_PI) {
		angles.x = 0.499f * M_PI;
	}

	if (angles.x <= -0.499f * M_PI) {
		angles.x = -0.499f * M_PI;
	}

	// get the yaw in the [-pi, pi] range
	angles.y = std::fmod(std::fmod(angles.y, 2 * M_PI) + 3 * M_PI, 2 * M_PI) - M_PI;

	camera.SetRotation(angles);
}

void EulerCameraController::_Move(Camera& camera, float dt) {
	// get the movement
	vec4 movement = vec4();
	bool hasMovement = false;

	if (IsKeyPressed(Input::Key::KEY_W)) { movement.z -= _velocity_z; hasMovement = true; }
	if (IsKeyPressed(Input::Key::KEY_S)) { movement.z += _velocity_z; hasMovement = true; }
	if (IsKeyPressed(Input::Key::KEY_A)) { movement.x -= _velocity_x; hasMovement = true; }
	if (IsKeyPressed(Input::Key::KEY_D)) { movement.x += _velocity_x; hasMovement = true; }

	if (hasMovement) {
		// rotate the movement vector so that it is pointed in the direction
		// the camera is facing
		movement = quat(camera.Rotation()) * movement;
		camera.Move(movement * dt);
	}
}

}
