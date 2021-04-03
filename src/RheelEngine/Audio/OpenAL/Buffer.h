/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_AL_BUFFER_H
#define RHEELENGINE_AL_BUFFER_H
#include "../../_common.h"

#include <AL/al.h>

namespace rheel::al {

class RE_API Buffer {

public:
	Buffer();

	ALuint GetName() const;

	void SetData(ALenum format, const ALvoid* data, ALsizei size, ALsizei frequency);

private:
	std::shared_ptr<ALuint> _handle;

};

}

#endif
