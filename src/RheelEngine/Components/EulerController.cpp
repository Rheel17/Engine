/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "EulerController.h"

namespace rheel {

// modify the glm::eulerAngles(quat) formula to always return Euler angles that
// fall in the range of the Euler Controller.
static vec3 euler(const quat& q) {
	vec3 result = glm::eulerAngles(q);
	bool flip = true;

	// check if the pitch is out of range
	if (result.x < -M_PI / 2.0f) {
		result.x += M_PI;
	} else if (result.x > M_PI / 2.0f) {
		result.x -= M_PI;
	} else {
		flip = false;
	}

	if (flip) {
		// we flipped the pitch, so do the same for yaw and roll.

		if (result.y <= 0) {
			result.y = -M_PI - result.y;
		} else {
			result.y = M_PI - result.y;
		}

		if (result.z <= 0) {
			result.z += M_PI;
		} else {
			result.z -= M_PI;
		}
	}

	return result;
}

void EulerController::Activate() {
	SetReceiveInput(true);
}

void EulerController::Deactivate() {
	SetReceiveInput(false);
}

void EulerController::Update() {
	float dt = GetTimeDelta();
	_Rotate(dt);
	_Move(dt);
}

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

void EulerController::_Rotate(float dt) {
	vec2 mouse = GetMouseDelta();

	float yaw = glm::radians(mouse.x * _velocity_yaw / 100.0f);
	float pitch = glm::radians(mouse.y * _velocity_pitch / 100.0f);

	if (yaw == 0 && pitch == 0) {
		return;
	}

	vec3 angles = euler(GetParent()->transform.GetRotation());

	angles.x -= pitch;
	angles.y -= yaw;
	angles.z = 0.0f;

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

	if (IsKeyPressed(Input::Key::KEY_W)) { movement.z -= _velocity_z; hasMovement = true; }
	if (IsKeyPressed(Input::Key::KEY_S)) { movement.z += _velocity_z; hasMovement = true; }
	if (IsKeyPressed(Input::Key::KEY_A)) { movement.x -= _velocity_x; hasMovement = true; }
	if (IsKeyPressed(Input::Key::KEY_D)) { movement.x += _velocity_x; hasMovement = true; }

	if (hasMovement) {
		// rotate the movement vector so that it is pointed in the direction
		// the camera is facing
		movement = GetParent()->transform.GetRotation() * movement;
		GetParent()->transform.Move(movement * dt);
	}
}

}
