/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ImageTexture.h"

#include "../Engine.h"

namespace rheel {

std::unordered_map<std::tuple<std::uintptr_t, ImageTexture::WrapType, bool>, ImageTexture> ImageTexture::_texture_cache;

void ImageTexture::Bind(unsigned textureUnit) const {
	_texture.Bind(textureUnit);
}

ImageTexture::ImageTexture(const Image& image, WrapType type, bool linear) {
	if (linear) {
		if (Engine::GetDisplayConfiguration().enable_mipmaps) {
			_texture.SetMinifyingFilter(GL::Texture::FilterFunction::LINEAR_MIPMAP_LINEAR);
		} else {
			_texture.SetMinifyingFilter(GL::Texture::FilterFunction::LINEAR);
		}

		_texture.SetMagnificationFilter(GL::Texture::FilterFunction::LINEAR);
	} else {
		if (Engine::GetDisplayConfiguration().enable_mipmaps) {
			_texture.SetMinifyingFilter(GL::Texture::FilterFunction::NEAREST_MIPMAP_NEAREST);
		} else {
			_texture.SetMinifyingFilter(GL::Texture::FilterFunction::NEAREST);
		}
		_texture.SetMagnificationFilter(GL::Texture::FilterFunction::NEAREST);
	}

	switch (type) {
		case WrapType::WRAP:
			_texture.SetWrapParameterS(GL::Texture::WrapParameter::REPEAT);
			_texture.SetWrapParameterT(GL::Texture::WrapParameter::REPEAT);
			break;
		case WrapType::CLAMP:
			_texture.SetWrapParameterS(GL::Texture::WrapParameter::CLAMP_TO_EDGE);
			_texture.SetWrapParameterT(GL::Texture::WrapParameter::CLAMP_TO_EDGE);
			break;

	}

	_texture.SetAnisotropyParameter(Engine::GetDisplayConfiguration().anisotropic_level);

	unsigned w = image.GetWidth(), h = image.GetHeight();
	const float *data = image.GetRawColorData();
	auto glData = new float[w * h * 4];

	// reverse the y-coordinate of the image for _OpenGL.
	for (unsigned y = 0; y < h; y++) {
		memcpy(glData + y * w * 4, data + (h - y - 1) * w * 4, w * 4 * sizeof(float));
	}

	_texture.SetData(GL::InternalFormat::RGBA, image.GetWidth(), image.GetHeight(), GL::Format::RGBA, data);

	if (Engine::GetDisplayConfiguration().enable_mipmaps) {
		_texture.GenerateMipmap();
	}

	delete[] glData;
}

const ImageTexture& ImageTexture::Get(const Image& image, WrapType type, bool linear) {
	auto tuple = std::make_tuple(image.GetAddress(), type, linear);
	auto iter = _texture_cache.find(tuple);

	if (iter == _texture_cache.end()) {
		iter = _texture_cache.emplace(tuple, ImageTexture(image, type, linear)).first;
	}

	return iter->second;
}

}
