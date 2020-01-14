/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SOUND_H_
#define SOUND_H_
#include "_common.h"

#include <AL/al.h>

#include <iostream>

namespace rheel {

class Sound {
	RE_NO_MOVE(Sound);
	RE_NO_COPY(Sound);

public:
	enum SoundFormat {
		FormatWAV
	};

	enum class InternalFormat {
		MONO8 = AL_FORMAT_MONO8, MONO16 = AL_FORMAT_MONO16,
		STEREO8 = AL_FORMAT_STEREO8, STEREO16 = AL_FORMAT_STEREO16
	};

public:
	Sound(const std::string& filename, SoundFormat format);
	~Sound();

	const char *Data() const;

	unsigned Size() const;

	InternalFormat Format() const;

	int Frequency() const;

private:
	void _LoadWAVFile(const std::string& filename);

	void _LoadFromFile(const std::string& filename, SoundFormat format);

	void *_data;
	unsigned _size;

	InternalFormat _format;
	int _frequency;

};

}

#endif
