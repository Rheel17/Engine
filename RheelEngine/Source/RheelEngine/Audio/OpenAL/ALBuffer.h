/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef ALBUFFER_H_
#define ALBUFFER_H_
#include "../../_common.h"

#include <AL/al.h>

namespace rheel {

class RE_API ALBuffer {

public:
	ALBuffer();

	ALuint ID() const;

	void SetData(ALenum format, const ALvoid *data, ALsizei size, ALsizei frequency);

private:
	std::shared_ptr<ALuint> _handle;

};

}

#endif
