/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef IMAGE3DRESOURCE_H_
#define IMAGE3DRESOURCE_H_
#include "../_common.h"

#include "Resource.h"
#include "Image3D.h"

namespace rheel {

class RE_API Image3DResource : public Resource<Image3D> {
	RE_NO_COPY(Image3DResource);
	RE_NO_MOVE(Image3DResource);

	friend class ResourceManager;

public:

private:
	Image3DResource(const std::string& path);
	Image3DResource(const std::string& path, Image3D *image);

};

}

#endif
