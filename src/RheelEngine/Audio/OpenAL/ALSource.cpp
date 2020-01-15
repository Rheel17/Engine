/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ALSource.h"

namespace rheel {

ALSource::ALSource() {
	auto *id = new ALuint;
	alGenSources(1, id);

	_handle = std::shared_ptr<ALuint>(id, [](ALuint *id) {
		alDeleteSources(1, id);
		delete id;
	});
}

void ALSource::SetBuffer(const ALBuffer& buffer) {
	alSourcei(*_handle, AL_BUFFER, buffer.ID());
}

void ALSource::SetGain(float gain) {
	alSourcef(*_handle, AL_GAIN, gain);
}

void ALSource::SetLooping(bool looping) {
	alSourcei(*_handle, AL_LOOPING, looping);
}

void ALSource::SetPosition(const vec3& position) {
	alSource3f(*_handle, AL_POSITION, position.x, position.y, position.z);
}

void ALSource::SetVelocity(const vec3& velocity) {
	alSource3f(*_handle, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void ALSource::Play() {
	alSourcePlay(*_handle);
}

void ALSource::Stop() {
	alSourceStop(*_handle);
}

}