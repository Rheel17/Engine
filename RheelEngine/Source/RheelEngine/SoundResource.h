/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef SOUNDRESOURCE_H_
#define SOUNDRESOURCE_H_
#include "_common.h"

#include "Sound.h"
#include "Resource.h"

namespace rheel {

class RE_API SoundResource : public Resource<Sound> {
	RE_NO_COPY(SoundResource);
	RE_NO_MOVE(SoundResource);

	friend class ResourceManager;

private:
	SoundResource(const std::string& path);

};

}

#endif
