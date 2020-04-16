/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Sound.h"

#include <fstream>

namespace rheel {

Sound::Sound(std::vector<char> data, InternalSoundFormat format, int sampleFrequency) :
		Asset({ std::move(data), format, sampleFrequency }) {}

const char* Sound::GetRawSampleData() const {
	return GetRaw()->data.data();
}

unsigned Sound::GetRawSampleSize() const {
	return GetRaw()->data.size();
}

InternalSoundFormat Sound::GetFormat() const {
	return GetRaw()->format;
}

int Sound::GetSampleFrequency() const {
	return GetRaw()->sample_frequency;
}

}
