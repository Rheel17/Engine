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
	Image _DoLoad(const std::string& path) override;

private:
	PngLoader() = default;

	static Image _LoadPNG(std::istream& input);



};

}

#endif