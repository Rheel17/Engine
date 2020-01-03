/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "AudioManager.h"

#include <AL/alc.h>
#include <AL/al.h>
#include <AL/alext.h>

namespace rheel {

AudioManager::AudioManager() {
	const ALCchar *defaultDevice = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
	Log::Info() << "Audio device: " << defaultDevice << std::endl;

	// TODO: handle errors

	_device = alcOpenDevice(defaultDevice);
	if (!_device) {
		Log::Error() << "Could not create audio device" << std::endl;
		return;
	}

	_context = alcCreateContext(_device, nullptr);
	if (!_context) {
		Log::Error() << "Could not create audio context" << std::endl;
		return;
	}

	if (!alcMakeContextCurrent(_context)) {
		Log::Error() << "Could not make audio context current" << std::endl;
		return;
	}
}

AudioManager::~AudioManager() {
	alcDestroyContext(_context);
	alcCloseDevice(_device);
}

AudioSource *AudioManager::Play(const SoundResource& resource) {
	const AudioClip& clip = resource.GetAudioClip();

	AudioSource *src = new AudioSource(this, clip);
	auto source = std::unique_ptr<AudioSource>(src);

	source->Play();

	_sources.push_back(std::move(source));
	return src;
}

AudioSource *AudioManager::Loop(const SoundResource& resource) {
	const AudioClip& clip = resource.GetAudioClip();

	AudioSource *src = new AudioSource(this, clip);
	auto source = std::unique_ptr<AudioSource>(src);

	source->Loop();

	_sources.push_back(std::move(source));
	return src;
}

void AudioManager::Stop(AudioSource *source) {
	source->_Stop();

	auto iter = std::find_if(_sources.begin(), _sources.end(),
			[source](const auto& ptr) { return ptr.get() == source; });
	_sources.erase(iter);
}

ALListener& AudioManager::GlobalListener() {
	return ALListener::INSTANCE;
}

void AudioManager::_StopAll() {
	for (const auto& source : _sources) {
		source->_Stop();
	}

	_sources.clear();
}

}
