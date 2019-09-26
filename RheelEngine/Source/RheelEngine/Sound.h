/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef SOUND_H_
#define SOUND_H_
#include "_common.h"

#include <string.h>

#include <iostream>

namespace rheel {

class Sound {
	RE_NO_MOVE(Sound);
	RE_NO_COPY(Sound);

public:
	enum SoundFormat {
		FormatWAV
	};

public:
	Sound(const std::string& filename, SoundFormat format);
	~Sound();

	const char *Data() const;

private:
	void _LoadWAV(const char *input);

	void _LoadFromFile(const std::string& filename, SoundFormat format);
	void _LoadFromMemory(const char *input, SoundFormat format);

	char *_data;

};

}

#endif
