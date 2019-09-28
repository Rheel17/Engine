/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef AUDIOSOURCE_H_
#define AUDIOSOURCE_H_
#include "../_common.h"

#include "AudioClip.h"
#include "OpenAL/ALSource.h"

namespace rheel {

class AudioManager;

class RE_API AudioSource {
	RE_NO_MOVE(AudioSource);
	RE_NO_COPY(AudioSource);

	friend class AudioManager;

public:
	void Play();
	void Loop();
	void Stop();

	/**
	 * Sets the gain of the source. This gain can be in a range of 0 to 1, and
	 * values outside of this range will be clamped.
	 */
	void SetGain(float gain);

	/**
	 * Sets the position of the source
	 */
	void SetPosition(const vec3& position);

	/**
	 * Sets the velocity of the source
	 */
	void SetVelocity(const vec3& velocity);

private:
	AudioSource(AudioManager *manager, const AudioClip& clip);

	void _Stop();

	AudioManager *_manager;
	ALSource _source;

};

}

#endif
