/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_EULERCONTROLLER_H
#define RHEELENGINE_EULERCONTROLLER_H
#include "../_common.h"

#include "../Component.h"

namespace rheel {

class RE_API EulerController : public Component {

public:
	// gen_component_id

	void Update() override;

	/**
	 * Sets the angular velocity of the camera. This is the speed with which the
	 * camera turns if the user moves the mouse.
	 */
	void SetAngularVelocity(float velocity);

	/**
	 * Sets the angular velocity of the camera. This is the speed with which the
	 * camera turns if the user moves the mouse.
	 *
	 * This variant of the method allows for separate yaw and pitch velocities.
	 */
	void SetAngularVelocity(float yawVelocity, float pitchVelocity);

	/**
	 * Sets the velocity of the camera. This is the speed with which the camera
	 * moves through the scene.
	 */
	void SetVelocity(float velocity);

	/**
	 * Sets the velocity of the camera. This is the speed with which the camera
	 * moves through the scene.
	 *
	 * This variant of the method allows for separate forward/backward and
	 * sideways speeds.
	 */
	void SetVelocity(float forwardsBackwardsVelocity, float sideVelocity);

private:
	void Rotate_(float dt);
	void Move_(float dt);

	float _velocity_yaw = 10.0f;
	float _velocity_pitch = 10.0f;
	float _velocity_z = 3.5f;
	float _velocity_x = 3.5f;
};

}

#endif
