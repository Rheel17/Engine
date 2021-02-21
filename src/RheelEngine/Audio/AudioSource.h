/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_AUDIOSOURCE_H
#define RHEELENGINE_AUDIOSOURCE_H
#include "../_common.h"

#include "AudioClip.h"
#include "OpenAL/Source.h"

namespace rheel {

class AudioManager;

class RE_API AudioSource {
	RE_NO_MOVE(AudioSource);
	RE_NO_COPY(AudioSource);

	friend class AudioManager;

public:
	AudioSource(AudioManager& manager, const AudioClip& clip);
	~AudioSource() = default;

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
	void _stop();

	AudioManager& _manager;
	al::Source _source;

};

}

#endif
