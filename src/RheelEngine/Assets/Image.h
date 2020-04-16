/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_IMAGE_H
#define RHEELENGINE_IMAGE_H
#include "../_common.h"

#include "Asset.h"
#include "../Color.h"

namespace rheel {

struct image_data {
	unsigned width;
	unsigned height;
	std::vector<Color> pixels;
};

class RE_API Image : public Asset<image_data> {

public:
	/**
	 * Creates an empty image with the given dimensions
	 */
	Image(unsigned width, unsigned height);

	/*
	 * Creates an image with the given pixel data
	 */
	Image(unsigned width, unsigned height, std::vector<Color> pixels);

	unsigned GetWidth() const;
	unsigned GetHeight() const;

	const Color& At(unsigned x, unsigned y) const;
	Color& At(unsigned x, unsigned y);

	Image SubImage(unsigned x, unsigned y, unsigned width, unsigned height) const;

	const float* GetRawColorData() const;

private:
	Image() = default;
	Image(std::nullptr_t null) :
			Asset(nullptr) {}

public:
	static Image Null();

};

}

#endif
