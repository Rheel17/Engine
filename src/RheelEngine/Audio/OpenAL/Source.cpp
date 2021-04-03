/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Source.h"

namespace rheel::al {

Source::Source() {
	auto* id = new ALuint;
	alGenSources(1, id);

	_handle = std::shared_ptr<ALuint>(id, [](ALuint* id) {
		alDeleteSources(1, id);
		delete id;
	});
}

void Source::SetBuffer(const Buffer& buffer) {
	alSourcei(*_handle, AL_BUFFER, buffer.GetName());
}

void Source::SetGain(float gain) {
	alSourcef(*_handle, AL_GAIN, gain);
}

void Source::SetLooping(bool looping) {
	alSourcei(*_handle, AL_LOOPING, looping);
}

void Source::SetPosition(const vec3& position) {
	alSource3f(*_handle, AL_POSITION, position.x, position.y, position.z);
}

void Source::SetVelocity(const vec3& velocity) {
	alSource3f(*_handle, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void Source::Play() {
	alSourcePlay(*_handle);
}

void Source::Stop() {
	alSourceStop(*_handle);
}

}
