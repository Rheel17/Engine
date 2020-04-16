/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_SOUND_H
#define RHEELENGINE_SOUND_H
#include "../_common.h"

#include <al/al.h>

#include "Asset.h"

namespace rheel {

enum class InternalSoundFormat {
	MONO8 = AL_FORMAT_MONO8, MONO16 = AL_FORMAT_MONO16,
	STEREO8 = AL_FORMAT_STEREO8, STEREO16 = AL_FORMAT_STEREO16
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
