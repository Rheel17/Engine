/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Sound.h"

#include <AL/alut.h>

namespace rheel {

void Sound::_LoadWAV(const char *input) {
	ALenum format;
	ALsizei size;
	ALsizei frequency;
	ALboolean loop;

	// TODO: look into other .wav parsing libraries that are not deprecated
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

	alutLoadWAVMemory(
			reinterpret_cast<ALbyte *>(const_cast<char *>(input)),
			&format, reinterpret_cast<ALvoid **>(&_data), &size, &frequency, &loop);

#pragma GCC diagnostic pop

}

}

