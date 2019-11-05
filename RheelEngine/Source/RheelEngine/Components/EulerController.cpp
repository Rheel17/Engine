/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "EulerController.h"

namespace rheel {

void EulerController::SetAngularVelocity(float velocity) {
	SetAngularVelocity(velocity, velocity);
}

void EulerController::SetAngularVelocity(float yawVelocity, float pitchVelocity) {
	_velocity_yaw = yawVelocity;
	_velocity_pitch = pitchVelocity;
}

void EulerController::SetVelocity(float velocity) {
	SetVelocity(velocity, velocity);
}

void EulerController::SetVelocity(float forwardsBackwardsVelocity, float sideVelocity) {
	_velocity_z = forwardsBackwardsVelocity;
	_velocity_x = sideVelocity;
}


void EulerController::Update() {
	// TODO: get dt
	_Rotate(1.0 / 60.0);
	_Move(1.0 / 60.0);
}

void EulerController::_Rotate(float dt) {
	vec2 mouse = vec2(0, 0);
	// TODO: get mouse
	// vec2 mouse = InputMouseDelta();

	float yaw = glm::radians(mouse.x * _velocity_yaw / 100.0f);
	float pitch = glm::radians(mouse.y * _velocity_pitch / 100.0f);

	vec3 angles = glm::eulerAngles(GetParent()->transform.GetRotation());
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

	GetParent()->transform.SetRotation(angles);
}

void EulerController::_Move(float dt) {
	// get the movement
	vec4 movement = vec4();
	bool hasMovement = false;

//	if (IsKeyPressed(Input::Key::KEY_W)) { movement.z -= _velocity_z; hasMovement = true; }
//	if (IsKeyPressed(Input::Key::KEY_S)) { movement.z += _velocity_z; hasMovement = true; }
//	if (IsKeyPressed(Input::Key::KEY_A)) { movement.x -= _velocity_x; hasMovement = true; }
//	if (IsKeyPressed(Input::Key::KEY_D)) { movement.x += _velocity_x; hasMovement = true; }

	if (hasMovement) {
		// rotate the movement vector so that it is pointed in the direction
		// the camera is facing
		movement = GetParent()->transform.GetRotation() * movement;
		GetParent()->transform.Move(movement * dt);
	}
}

}
