/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "ALListener.h"

namespace rheel {

void ALListener::SetGain(float gain) {
	alListenerf(AL_GAIN, std::clamp(gain, 0.0f, 1.0f));
}

void ALListener::SetPosition(const vec3& position) {
	alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void ALListener::SetVelocity(const vec3& velocity) {
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void ALListener::SetOrientation(const vec3& forward, const vec3& up) {
	float orientation[6] { forward.x, forward.y, forward.z, up.x, up.y, up.z };
	alListenerfv(AL_ORIENTATION, orientation);
}

void ALListener::SetOrientation(const quat& rotation) {
	SetOrientation(rotation * vec4(0, 0, -1, 0), rotation * vec4(0, 1, 0, 0));
}

}
