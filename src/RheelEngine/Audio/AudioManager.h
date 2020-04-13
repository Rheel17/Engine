/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_
#include "../_common.h"

#include <AL/alc.h>

#include "AudioSource.h"
#include "OpenAL/ALListener.h"

namespace rheel {

class RE_API AudioManager {
	RE_NO_MOVE(AudioManager);
	RE_NO_COPY(AudioManager);

	friend class Engine;

public:
	AudioManager();
	~AudioManager();

	/**
	 * Plays a sound. The returned AudioSource pointer can be used to control
	 * the sound.
	 */
	AudioSource *Play(const Sound& sound);

	/**
	 * Plays a sound, looping when it reaches the end. The returned AudioSource
	 * pointer can be used to control the sound.
	 */
	AudioSource *Loop(const Sound& sound);

	/**
	 * Stops the sound. When calling this method, the audio source is deleted,
	 * and the pointer is deleted.
	 */
	void Stop(AudioSource *source);

	/**
	 * Returns the global audio listener.
	 */
	ALListener& GetListener();

private:
	const AudioClip& _GetAudioClip(const Sound& sound);
	void _StopAll();

	ALCdevice *_device;
	ALCcontext *_context;

	ALListener _listener;

	std::vector<std::unique_ptr<AudioSource>> _sources;
	std::unordered_map<std::uintptr_t, AudioClip> _clip_cache;

};

}

#endif
