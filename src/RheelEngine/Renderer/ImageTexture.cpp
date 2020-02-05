/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ImageTexture.h"

namespace rheel {

std::unordered_map<std::uintptr_t, ImageTexture> ImageTexture::_texture_cache;

void ImageTexture::Bind(unsigned textureUnit) const {
	_texture.Bind(textureUnit);
}

ImageTexture::ImageTexture(const Image& image)
		: _texture(image.GetWidth(), image.GetHeight(), GL_RGBA) {

	_texture.SetMinifyingFilter(GL::FilterFunction::LINEAR);
	_texture.SetMagnificationFilter(GL::FilterFunction::LINEAR);
	_texture.SetWrapParameterS(GL::WrapParameter::REPEAT);
	_texture.SetWrapParameterT(GL::WrapParameter::REPEAT);

	unsigned w = image.GetWidth(), h = image.GetHeight();
	const float *data = image.GetRawColorData();
	auto glData = new float[w * h * 4];

	// reverse the y-coordinate of the image for OpenGL.
	for (unsigned y = 0; y < h; y++) {
		memcpy(glData + y * w * 4, data + (h - y - 1) * w * 4, w * 4 * sizeof(float));
	}

	_texture.SetData(GL_RGBA, GL_FLOAT, glData);

	delete[] glData;
}

const ImageTexture& ImageTexture::Get(const Image& image) {
	auto iter = _texture_cache.find(image.GetAddress());

	if (iter == _texture_cache.end()) {
		iter = _texture_cache.emplace(image.GetAddress(), ImageTexture(image)).first;
	}

	return iter->second;
}

}
