/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "AudioClip.h"

namespace rheel {

AudioClip::AudioClip(const Sound& sound) {
	_buffer.SetData(ALenum(sound.Format()), sound.Data(), sound.Size(), sound.Frequency());
}

const ALBuffer& AudioClip::Buffer() const {
	return _buffer;
}

}
