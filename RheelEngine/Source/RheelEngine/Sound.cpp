/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Sound.h"

#include <fstream>

namespace rheel {

Sound::Sound(const std::string& filename, SoundFormat format) {
	_LoadFromFile(filename, format);
}

Sound::~Sound() {}

const char *Sound::Data() const {
	return _data;
}

void Sound::_LoadFromFile(const std::string& filename, SoundFormat format) {
	std::ifstream f(filename);

	if (!f) {
		throw std::runtime_error("Error while reading sound file: " + filename);
	}

	std::vector<char> data;
	std::streamsize block = 2048;
	std::streamsize count = 0;
	char buffer[block];

	do {
		f.read(buffer, block);
		count = f.gcount();
		data.insert(data.end(), buffer, buffer + count);
	} while (count == block);

	_LoadFromMemory(data.data(), format);
}

void Sound::_LoadFromMemory(const char *input, SoundFormat format) {
	switch (format) {
		case FormatWAV:
			_LoadWAV(input);
			break;
	}
}

}
