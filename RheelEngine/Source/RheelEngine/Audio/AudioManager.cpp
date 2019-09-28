/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "AudioManager.h"

#include <AL/alc.h>
#include <AL/al.h>
#include <AL/alext.h>

namespace rheel {

AudioManager::AudioManager() {
	const ALCchar *defaultDevice = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
	std::cout << "Audio device: " << defaultDevice << std::endl;

	_device = alcOpenDevice(defaultDevice);
	if (!_device) {
		throw std::runtime_error("Could not create audio device");
	}

	_context = alcCreateContext(_device, nullptr);
	if (!_context) {
		throw std::runtime_error("Could not create audio context");
	}

	if (!alcMakeContextCurrent(_context)) {
		throw std::runtime_error("Could not make audio context current");
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

void AudioManager::Stop(AudioSource *source) {
	source->_Stop();

	auto iter = std::find_if(_sources.begin(), _sources.end(),
			[source](const auto& ptr) { return ptr.get() == source; });
	_sources.erase(iter);
}

void AudioManager::_StopAll() {
	for (const auto& source : _sources) {
		source->_Stop();
	}

	_sources.clear();
}

}
