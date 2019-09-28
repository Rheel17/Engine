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
	void Stop();

private:
	AudioSource(AudioManager *manager, const AudioClip& clip);

	void _Stop();

	AudioManager *_manager;
	ALSource _source;

};

}

#endif
