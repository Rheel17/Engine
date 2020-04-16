/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Listener.h"

namespace rheel::al {

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStatic"

void Listener::SetGain(float gain) {
	alListenerf(AL_GAIN, gain);
}

void Listener::SetPosition(const vec3& position) {
	alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void Listener::SetVelocity(const vec3& velocity) {
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void Listener::SetOrientation(const vec3& forward, const vec3& up) {
	float orientation[6]{ forward.x, forward.y, forward.z, up.x, up.y, up.z };
	alListenerfv(AL_ORIENTATION, orientation);
}

void Listener::SetOrientation(const quat& rotation) {
	// TODO: look into this
	SetOrientation(rotation * vec4(0, 0, -1, 0), rotation * vec4(0, 1, 0, 0));
}

#pragma clang diagnostic pop

}