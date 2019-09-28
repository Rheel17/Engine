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
	_source.SetLooping(false);
	_source.Play();
}

void AudioSource::Loop() {
	_source.SetLooping(true);
	_source.Play();
}

void AudioSource::Stop() {
	_manager->Stop(this);
}

void AudioSource::SetGain(float gain) {
	_source.SetGain(gain);
}

void AudioSource::SetPosition(const vec3& position) {
	_source.SetPosition(position);
}

void AudioSource::SetVelocity(const vec3& velocity) {
	_source.SetVelocity(velocity);
}

void AudioSource::_Stop() {
	_source.Stop();
}

}
