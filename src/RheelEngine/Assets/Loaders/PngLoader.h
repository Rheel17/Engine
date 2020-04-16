/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_PNGLOADER_H
#define RHEELENGINE_PNGLOADER_H
#include "../../_common.h"

#include "Loader.h"
#include "../Image.h"

namespace rheel {

class RE_API PngLoader : public Loader<Image> {
	friend class AssetLoader;

protected:
	Image DoLoad(const std::string& path) const override;

private:
	PngLoader() = default;

	static Image LoadPng_(std::istream& input);

};

}

#endif