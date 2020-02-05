/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_WAVELOADER_H
#define RHEELENGINE_WAVELOADER_H
#include "../../_common.h"

#include "Loader.h"
#include "../Sound.h"

namespace rheel {

class RE_API WaveLoader : public Loader<Sound> {
	friend class AssetLoader;

protected:
	Sound _DoLoad(const std::string& path) override;

private:
	WaveLoader() = default;

};

}

#endif