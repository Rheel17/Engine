/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef AUDIOCLIP_H_
#define AUDIOCLIP_H_
#include "../_common.h"

#include "../Sound.h"
#include "OpenAL/ALBuffer.h"

namespace rheel {

class RE_API AudioClip {

public:
	AudioClip(const Sound& sound);

	const ALBuffer& Buffer() const;

private:
	ALBuffer _buffer;

};

}

#endif
