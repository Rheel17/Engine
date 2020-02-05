/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "WaveLoader.h"

#include <AL/alut.h>

namespace rheel {


Sound WaveLoader::_DoLoad(const std::string& path) const {
	// copy the file name into a non-const ALbyte array for alut to handle
	std::size_t len = path.length();
	ALbyte file[len + 1];

	for (std::size_t i = 0; i <= len; i++) {
		file[i] = path[i];
	}

	ALenum format;
	ALsizei size;
	ALsizei frequency;
	ALboolean loop;
	ALvoid *rawData;

	// TODO: look into other .wav parsing libraries that are not deprecated
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

	alutLoadWAVFile(file, &format, &rawData, &size, &frequency, &loop);

	// copy the data into a C++ vector
	std::vector<char> data(reinterpret_cast<char *>(rawData), reinterpret_cast<char *>(rawData) + size);

	// destroy the alut buffer again
	alutUnloadWAV(format, rawData, size, frequency);

#pragma clang diagnostic pop
#pragma GCC diagnostic pop

	return Sound(std::move(data), InternalSoundFormat(format), frequency);
}

}

