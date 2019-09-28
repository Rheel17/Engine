/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "AudioSource.h"

#include "AudioManager.h"

namespace rheel {

AudioSource::AudioSource(AudioManager *manager, const AudioClip& clip) :
		_manager(manager) {

	_source.SetBuffer(clip.Buffer());
}

void AudioSource::Play() {
	_source.Play();
}

void AudioSource::Stop() {
	_manager->Stop(this);
}

void AudioSource::_Stop() {
	_source.Stop();
}

}
