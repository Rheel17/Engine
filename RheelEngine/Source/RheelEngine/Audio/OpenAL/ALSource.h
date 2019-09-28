/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef ALSOURCE_H_
#define ALSOURCE_H_
#include "../../_common.h"

#include "ALBuffer.h"

#include <memory>

namespace rheel {

class RE_API ALSource {

public:
	ALSource();

	/**
	 * Sets the sound buffer for this source.
	 */
	void SetBuffer(const ALBuffer& buffer);

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
