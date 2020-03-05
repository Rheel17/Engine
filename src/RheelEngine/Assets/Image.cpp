/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Image.h"

#include <iterator>

namespace rheel {

Image::Image(unsigned width, unsigned height) :
		Asset({ width, height, {} }) {

	_GetRaw()->pixels.resize(width * height);
}

Image::Image(unsigned width, unsigned height, std::vector<Color> pixels) :
		Asset({ width, height, std::move(pixels) }) {}

unsigned Image::GetWidth() const {
	return _GetRaw()->width;
}

unsigned Image::GetHeight() const {
	return _GetRaw()->height;
}

const Color& Image::At(unsigned x, unsigned y) const {
	auto data = _GetRaw();
	return data->pixels[y * data->width + x];
}

Color& Image::At(unsigned x, unsigned y) {
	auto data = _GetRaw();
	return data->pixels[y * data->width + x];
}

Image Image::SubImage(unsigned x, unsigned y, unsigned width, unsigned height) const {
	Image image(width, height);
	for (unsigned i = 0; i < width; i++) {
		for (unsigned j = 0; j < height; j++) {
			image.At(i, j) = At(i + x, j + y);
		}
	}

	return std::move(image);
}

const float *Image::GetRawColorData() const {
	return reinterpret_cast<const float *>(_GetRaw()->pixels.data());
}

Image Image::Null() {
	return Image(nullptr);
}

}
