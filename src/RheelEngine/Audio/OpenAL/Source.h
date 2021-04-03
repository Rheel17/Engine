/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_AL_SOURCE_H
#define RHEELENGINE_AL_SOURCE_H
#include "../../_common.h"

#include "Buffer.h"

namespace rheel::al {

class RE_API Source {

public:
	Source();

	/**
	 * Sets the sound buffer for this source.
	 */
	void SetBuffer(const Buffer& buffer);

	/**
	 * Sets the gain of the source.
	 */
	void SetGain(float gain);

	/**
	 * Sets whether this source should loop.
	 */
	void SetLooping(bool looping);

	/**
	 * Sets the position of the source
	 */
	void SetPosition(const vec3& position);

	/**
	 * Sets the velocity of the source
	 */
	void SetVelocity(const vec3& velocity);

	/**
	 * Plays the source
	 */
	void Play();

	/**
	 * Stops the source
	 */
	void Stop();

private:
	std::shared_ptr<ALuint> _handle;

};

}

#endif
