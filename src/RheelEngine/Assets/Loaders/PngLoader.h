/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_PNGLOADER_H
#define RHEELENGINE_PNGLOADER_H
#include "../../_common.h"

#include "Loader.h"
#include "../Image.h"

namespace rheel {

class RE_API PngLoader : public AbstractLoader<Image> {
	friend class AssetLoader;

public:
	Image Load(const std::string& path) const override;

private:
	static Image _load_png(std::istream& input);

};

}

#endif