/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Sound.h"

#include <AL/alut.h>

namespace rheel {

void Sound::_LoadWAVFile(const std::string& filename) {
	// copy the file name into a non-const ALbyte array for alut to handle
	std::size_t len = filename.length();
	ALbyte file[len + 1];

	for (std::size_t i = 0; i <= len; i++) {
		file[i] = filename[i];
	}

	ALenum format;
	ALsizei size;
	ALsizei frequency;
	ALboolean loop;

	// TODO: look into other .wav parsing libraries that are not deprecated
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

	alutLoadWAVFile(file, &format, &_data, &size, &frequency, &loop);

#pragma GCC diagnostic pop

	_size = size;
	_format = Sound::InternalFormat(format);
	_frequency = frequency;

}

}

