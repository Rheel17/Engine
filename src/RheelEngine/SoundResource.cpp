/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "SoundResource.h"

namespace rheel {

SoundResource::SoundResource(const std::string& path) :
	 Resource(path, new Sound(path, Sound::FormatWAV)) {

	// TODO: automatically find correct model format based on file extension
}

const AudioClip& SoundResource::GetAudioClip() const {
	if (!_audio_clip) {
		_audio_clip = std::make_unique<AudioClip>(Get());
	}

	return *_audio_clip;
}

}
