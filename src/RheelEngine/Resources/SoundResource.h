/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SOUNDRESOURCE_H_
#define SOUNDRESOURCE_H_
#include "../_common.h"

#include "Sound.h"
#include "Resource.h"
#include "../Audio/AudioClip.h"

namespace rheel {

class RE_API SoundResource : public Resource<Sound> {
	RE_NO_COPY(SoundResource);
	RE_NO_MOVE(SoundResource);

	friend class ResourceManager;

public:
	const AudioClip& GetAudioClip() const;

private:
	SoundResource(const std::string& path);

	mutable std::unique_ptr<AudioClip> _audio_clip = nullptr;
};

}

#endif
