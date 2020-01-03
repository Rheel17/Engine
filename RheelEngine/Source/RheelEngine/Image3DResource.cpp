/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Image3DResource.h"

namespace rheel {

Image3DResource::Image3DResource(const std::string& path) :
		Resource(path, new Image3D(path)) {}

Image3DResource::Image3DResource(const std::string& path, Image3D *image) :
		Resource(path, image) {}

}
