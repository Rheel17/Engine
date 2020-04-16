/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_AL_LISTENER_H
#define RHEELENGINE_AL_LISTENER_H
#include "../../_common.h"

#include <AL/al.h>

namespace rheel {
class AudioManager;
}

namespace rheel::al {

class RE_API Listener {
	RE_NO_COPY(Listener);
	RE_NO_MOVE(Listener);

	friend class rheel::AudioManager;

public:
	/**
	 * Sets the gain of the OpenAL listener.
	 */
	void SetGain(float gain);

	/**
	 * Sets the position of the OpenAL listener
	 */
	void SetPosition(const vec3& position);

	/**
	 * Sets the velocity of the OpenAL listener
	 */
	void SetVelocity(const vec3& velocity);

	/**
	 * Sets the orientation of the OpenAL listener
	 */
	void SetOrientation(const vec3& forward, const vec3& up);

	/**
	 * Sets the orientation of the OpenAL listener with a quaternion rotation.
	 * The forward vector will be calculated as rotation * (0, 0, -1, 0) and the
	 * up vector will be calculated as rotation * (0, 1, 0).
	 */
	void SetOrientation(const quat& rotation);

private:
	Listener() = default;

};

}

#endif
