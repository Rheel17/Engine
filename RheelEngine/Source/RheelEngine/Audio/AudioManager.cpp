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

}
