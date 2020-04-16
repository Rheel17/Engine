/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "AudioClip.h"

namespace rheel {

AudioClip::AudioClip(const Sound& sound) {
	_buffer.SetData(ALenum(sound.GetFormat()), sound.GetRawSampleData(), sound.GetRawSampleSize(), sound.GetSampleFrequency());
}

const al::Buffer& AudioClip::Buffer() const {
	return _buffer;
}

}
