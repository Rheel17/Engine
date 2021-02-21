/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_SOUND_H
#define RHEELENGINE_SOUND_H
#include "../_common.h"

#include <AL/al.h>

#include "Asset.h"

namespace rheel {

enum class InternalSoundFormat {
	MONO_8 = AL_FORMAT_MONO8, MONO_16 = AL_FORMAT_MONO16,
	STEREO_8 = AL_FORMAT_STEREO8, STEREO_16 = AL_FORMAT_STEREO16
};

struct RE_API sound_data {
	std::vector<char> data;
	InternalSoundFormat format;
	int sample_frequency;
};

class Sound : public Asset<sound_data> {

public:
	Sound(std::vector<char> data, InternalSoundFormat format, int sampleFrequency);

	const char* GetRawSampleData() const;

	unsigned GetRawSampleSize() const;

	InternalSoundFormat GetFormat() const;

	int GetSampleFrequency() const;

};

}

#endif
