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
	// Set correct texture parameters
	if (linear) {
		if (Engine::GetDisplayConfiguration().enable_mipmaps) {
			_texture.SetMinifyingFilter(gl::Texture::FilterFunction::LINEAR_MIPMAP_LINEAR);
		} else {
			_texture.SetMinifyingFilter(gl::Texture::FilterFunction::LINEAR);
		}

		_texture.SetMagnificationFilter(gl::Texture::FilterFunction::LINEAR);
	} else {
		if (Engine::GetDisplayConfiguration().enable_mipmaps) {
			_texture.SetMinifyingFilter(gl::Texture::FilterFunction::NEAREST_MIPMAP_NEAREST);
		} else {
			_texture.SetMinifyingFilter(gl::Texture::FilterFunction::NEAREST);
		}
		_texture.SetMagnificationFilter(gl::Texture::FilterFunction::NEAREST);
	}

	switch (type) {
		case WrapType::WRAP: _texture.SetWrapParameterS(gl::Texture::WrapParameter::REPEAT);
			_texture.SetWrapParameterT(gl::Texture::WrapParameter::REPEAT);
			break;
		case WrapType::CLAMP: _texture.SetWrapParameterS(gl::Texture::WrapParameter::CLAMP_TO_EDGE);
			_texture.SetWrapParameterT(gl::Texture::WrapParameter::CLAMP_TO_EDGE);
			break;

	}

	_texture.SetAnisotropyParameter(Engine::GetDisplayConfiguration().anisotropic_level);

	// upload texure data to the GPU
	_texture.SetData(gl::InternalFormat::RGBA, image.GetWidth(), image.GetHeight(), gl::Format::RGBA, image.GetRawColorData());

	// generate mipmaps
	if (Engine::GetDisplayConfiguration().enable_mipmaps) {
		_texture.GenerateMipmap();
	}
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
