/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_EULERCONTROLLER_H
#define RHEELENGINE_EULERCONTROLLER_H
#include "../_common.h"

#include "InputComponent.h"

namespace rheel {

class RE_API EulerController : public InputComponent {

public:
	// gen_component_id
	static constexpr const ComponentId id = 65284;
	static constexpr const ComponentFlags flags = ComponentFlags::BUILTIN;

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
	void SetAngularVelocity(float yaw_velocity, float pitch_velocity);

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
	void SetVelocity(float forwards_backwards_velocity, float side_velocity);

private:
	void _rotate(float dt);
	void _move(float dt);

	float _velocity_yaw = 10.0f;
	float _velocity_pitch = 10.0f;
	float _velocity_z = 3.5f;
	float _velocity_x = 3.5f;
};

}

#endif
