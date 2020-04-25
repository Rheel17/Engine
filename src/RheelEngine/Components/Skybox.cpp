/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Skybox.h"

#include "../Engine.h"

namespace rheel {

Skybox::Skybox(std::array<Image, 6> images, float scale) :
		_images(std::move(images)),
		_scale(scale) {}

Skybox::Skybox(const Image& image, float scale) :
		_images(LoadImages_(image)),
		_scale(scale) {}

void Skybox::Activate() {
	if (GetParent()->scene->_skybox != nullptr) {
		Log::Error() << "Scene already has a skybox" << std::endl;
	} else {
		GetParent()->scene->_skybox = this;
	}
}

void Skybox::Deactivate() {
	GetParent()->scene->_skybox = nullptr;
}

const std::array<Image, 6>& Skybox::GetImages() const {
	return _images;
}

float Skybox::GetScale() const {
	return _scale;
}

std::array<Image, 6> Skybox::LoadImages_(const Image& image) {
	return std::array<Image, 6>{
		LoadImage_(image, 0),
		LoadImage_(image, 1),
		LoadImage_(image, 2),
		LoadImage_(image, 3),
		LoadImage_(image, 4),
		LoadImage_(image, 5)
	};
}

Image Skybox::LoadImage_(const Image& image, int part) {
	// get the coordinates of the texture in the skybox
	unsigned size = image.GetWidth() / 4;
	unsigned x = 0;
	unsigned y = 0;

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
