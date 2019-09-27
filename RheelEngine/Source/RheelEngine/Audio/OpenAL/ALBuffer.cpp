/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "ALBuffer.h"

namespace rheel {

ALBuffer::ALBuffer() {
	ALuint *id = new ALuint;
	alGenBuffers(1, id);

	_handle = std::shared_ptr<ALuint>(id, [](ALuint *id) {
		alDeleteBuffers(1, id);
		delete id;
	});
}

ALuint ALBuffer::ID() const {
	return *_handle;
}

void ALBuffer::SetData(ALenum format, const ALvoid *data, ALsizei size, ALsizei frequency) {
	alBufferData(*_handle, format, data, size, frequency);
}

}
