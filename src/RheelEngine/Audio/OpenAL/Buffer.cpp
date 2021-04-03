/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Buffer.h"

namespace rheel::al {

Buffer::Buffer() {
	auto* id = new ALuint;
	alGenBuffers(1, id);

	_handle = std::shared_ptr<ALuint>(id, [](ALuint* id) {
		alDeleteBuffers(1, id);
		delete id;
	});
}

ALuint Buffer::GetName() const {
	return *_handle;
}

void Buffer::SetData(ALenum format, const ALvoid* data, ALsizei size, ALsizei frequency) {
	alBufferData(*_handle, format, data, size, frequency);
}

}
