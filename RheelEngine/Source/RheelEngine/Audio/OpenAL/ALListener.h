/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef ALLISTENER_H_
#define ALLISTENER_H_
#include "../../_common.h"

#include <AL/al.h>

namespace rheel {

class RE_API ALListener {
	RE_NO_COPY(ALListener);
	RE_NO_MOVE(ALListener);

	friend class AudioManager;

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
	ALListener() = default;

private:
	static ALListener INSTANCE;

};

}

#endif
