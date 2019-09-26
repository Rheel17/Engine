/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "SoundResource.h"

namespace rheel {

SoundResource::SoundResource(const std::string& path) :
	 Resource(path, new Sound(path, Sound::FormatWAV)) {

	// TODO: automatically find correct model format based on file extension
}

}
