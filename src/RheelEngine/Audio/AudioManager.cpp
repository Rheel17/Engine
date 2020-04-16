/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "AudioManager.h"

#include <AL/alc.h>

namespace rheel {

AudioManager::AudioManager() {
	const ALCchar* defaultDevice = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
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

AudioSource* AudioManager::Play(const Sound& sound) {
	const AudioClip& clip = GetAudioClip_(sound);

	auto src = new AudioSource(this, clip);
	auto source = std::unique_ptr<AudioSource>(src);

	source->Play();

	_sources.push_back(std::move(source));
	return src;
}

AudioSource* AudioManager::Loop(const Sound& sound) {
	const AudioClip& clip = GetAudioClip_(sound);

	auto src = new AudioSource(this, clip);
	auto source = std::unique_ptr<AudioSource>(src);

	source->Loop();

	_sources.push_back(std::move(source));
	return src;
}

void AudioManager::Stop(AudioSource* source) {
	source->Stop_();

	auto iter = std::find_if(_sources.begin(), _sources.end(),
			[source](const auto& ptr) { return ptr.get() == source; });
	_sources.erase(iter);
}

al::Listener& AudioManager::GetListener() {
	return _listener;
}

const AudioClip& AudioManager::GetAudioClip_(const Sound& sound) {
	auto iter = _clip_cache.find(sound.GetAddress());

	if (iter == _clip_cache.end()) {
		iter = _clip_cache.emplace(sound.GetAddress(), sound).first;
	}

	return iter->second;
}

void AudioManager::StopAll_() {
	for (const auto& source : _sources) {
		source->Stop_();
	}

	_sources.clear();
}

}
