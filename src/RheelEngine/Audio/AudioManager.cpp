// /*
//  * Copyright (c) Levi van Rheenen. All rights reserved.
//  */
// #include "AudioManager.h"
//
// namespace rheel {
//
// AudioManager::AudioManager() {
// 	const ALCchar* default_device = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
// 	Log::Info() << "Audio device: " << default_device << std::endl;
//
// 	// TODO: handle errors
//
// 	_device = alcOpenDevice(default_device);
// 	if (!_device) {
// 		Log::Error() << "Could not create audio device" << std::endl;
// 		return;
// 	}
//
// 	_context = alcCreateContext(_device, nullptr);
// 	if (!_context) {
// 		Log::Error() << "Could not create audio context" << std::endl;
// 		return;
// 	}
//
// 	if (!alcMakeContextCurrent(_context)) {
// 		Log::Error() << "Could not make audio context current" << std::endl;
// 		return;
// 	}
// }
//
// AudioManager::~AudioManager() {
// 	alcDestroyContext(_context);
// 	alcCloseDevice(_device);
// }
//
// AudioSource& AudioManager::Play(const Sound& sound) {
// 	const AudioClip& clip = _get_audio_clip(sound);
//
// 	auto source = std::make_unique<AudioSource>(*this, clip);
// 	source->Play();
//
// 	_sources.push_back(std::move(source));
// 	return *_sources.back();
// }
//
// AudioSource& AudioManager::Loop(const Sound& sound) {
// 	const AudioClip& clip = _get_audio_clip(sound);
//
// 	auto source = std::make_unique<AudioSource>(*this, clip);
// 	source->Loop();
//
// 	_sources.push_back(std::move(source));
// 	return *_sources.back();
// }
//
// void AudioManager::Stop(AudioSource& source) {
// 	source._stop();
//
// 	auto iter = std::find_if(_sources.begin(), _sources.end(),
// 			[&](const auto& ptr) { return ptr.get() == &source; });
// 	_sources.erase(iter);
// }
//
// al::Listener& AudioManager::GetListener() {
// 	return _listener;
// }
//
// const AudioClip& AudioManager::_get_audio_clip(const Sound& sound) {
// 	auto iter = _clip_cache.find(sound.GetAddress());
//
// 	if (iter == _clip_cache.end()) {
// 		iter = _clip_cache.emplace(sound.GetAddress(), sound).first;
// 	}
//
// 	return iter->second;
// }
//
// void AudioManager::_stop_all() {
// 	for (const auto& source : _sources) {
// 		source->_stop();
// 	}
//
// 	_sources.clear();
// }
//
// }
