#ifndef EULERCAMERACONTROLLER_H_
#define EULERCAMERACONTROLLER_H_
#include "../_common.h"

#include "InputScript.h"
#include "../Camera.h"

namespace rheel {

class RE_API EulerCameraController : public InputScript {
	SCRIPT_INIT(EulerCameraController)

public:
	/**
	 * Sets the camera this script should affect
	 */
	void SetCamera(const std::string& camera);

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

	void PreOnUpdate() override;

private:
	void _Rotate(Camera& camera, float dt);
	void _Move(Camera& camera, float dt);

	std::string _camera;
	float _velocity_yaw = 10.0f;
	float _velocity_pitch = 10.0f;
	float _velocity_z = 3.5f;
	float _velocity_x = 3.5f;
};

}

#endif
