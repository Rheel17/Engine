/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef IMAGE_H_
#define IMAGE_H_
#include "../_common.h"

#include "Asset.h"
#include "../Color.h"

namespace rheel {

struct _ImageData {
	unsigned width;
	unsigned height;
	std::vector<Color> pixels;
};

class RE_API Image : public Asset<_ImageData> {

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

	const float *GetRawColorData() const;

private:
	Image() = default;
	Image(std::nullptr_t null) : Asset(nullptr) {}

public:
	static Image Null();


};

}

#endif
