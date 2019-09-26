/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_
#include "../_common.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace rheel {

class RE_API AudioManager {
	RE_NO_MOVE(AudioManager);
	RE_NO_COPY(AudioManager);

public:
	AudioManager();
	~AudioManager();

private:
	ALCdevice *_device;
	ALCcontext *_context;

};

}

#endif
