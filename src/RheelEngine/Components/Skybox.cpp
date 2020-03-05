/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Skybox.h"

#include "../Engine.h"

namespace rheel {

Skybox::Skybox(Image image, float scale) :
		_image(std::move(image)), _scale(scale) {}

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

const Image& Skybox::GetImage() const {
	return _image;
}

float Skybox::GetScale() const {
	return _scale;
}

}
