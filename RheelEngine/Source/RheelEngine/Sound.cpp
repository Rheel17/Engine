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
	return reinterpret_cast<const char *>(_data);
}

unsigned Sound::Size() const {
	return _size;
}

Sound::InternalFormat Sound::Format() const {
	return _format;
}

int Sound::Frequency() const {
	return _frequency;
}

void Sound::_LoadFromFile(const std::string& filename, SoundFormat format) {
	switch (format) {
		case FormatWAV:
			_LoadWAVFile(filename);
	}
}

}
