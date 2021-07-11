/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Skybox.h"

namespace rheel {

Skybox::Skybox(std::array<Image, 6> images, float scale) :
		_images(std::move(images)),
		_scale(scale) {

	auto width = _images[0].GetWidth();
	auto height = _images[0].GetHeight();

	for (size_t i = 1; i < 6; i++) {
		if (_images[i].GetWidth() != width || _images[i].GetHeight() != height) {
			Log::Error() << "Skybox image dimensions must match" << std::endl;
			abort();
		}
	}
}

Skybox::Skybox(const Image& image, float scale) :
		Skybox(_load_images(image), scale) {}

const std::array<Image, 6>& Skybox::GetImages() const {
	return _images;
}

float Skybox::GetScale() const {
	return _scale;
}

std::array<Image, 6> Skybox::_load_images(const Image& image) {
	return std::array<Image, 6>{
			_load_image(image, 0),
			_load_image(image, 1),
			_load_image(image, 2),
			_load_image(image, 3),
			_load_image(image, 4),
			_load_image(image, 5)
	};
}

Image Skybox::_load_image(const Image& image, int part) {
	// get the coordinates of the texture in the skybox
	unsigned size = image.GetWidth() / 4;
	unsigned x;
	unsigned y;

	switch (part) {
		case 0:
			x = size;
			y = size;
			break;
		case 1:
			x = 2 * size;
			y = size;
			break;
		case 2:
			x = 3 * size;
			y = size;
			break;
		case 3:
			x = 0;
			y = size;
			break;
		case 4:
			x = size;
			y = 0;
			break;
		case 5:
			x = size;
			y = 2 * size;
			break;
		default:
			abort();
	}

	// extract the texture
	return image.SubImage(x, y, size, size);
}

}
