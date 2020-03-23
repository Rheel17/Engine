/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Texture.h"

#include "Capabilities.h"
#include "State.h"

namespace rheel::GL {

Texture::Texture(Texture::Target target) :
		_target(target) {}

Texture::Target rheel::GL::Texture::GetTarget() const {
	return _target;
}

void Texture::Bind(unsigned int textureUnit) const {
	State::BindTexture(textureUnit, *this);
}

void Texture::SetMinifyingFilter(FilterFunction filterFunction) {
	Bind();
	glTexParameteri(GLenum(_target), GL_TEXTURE_MIN_FILTER, GLint(filterFunction));
}

void Texture::SetMagnificationFilter(FilterFunction filterFunction) {
	if (filterFunction != FilterFunction::NEAREST && filterFunction != FilterFunction::LINEAR) {
		throw std::invalid_argument("Magnification filterFunction must be NEAREST of LINEAR");
	}

	Bind();
	glTexParameteri(GLenum(_target), GL_TEXTURE_MAG_FILTER, GLint(filterFunction));
}

void Texture::SetAnisotropyParameter(float parameter) {
	if (parameter < 1.0f) {
		throw std::invalid_argument("Anisotropy parameter must be at least 1.0");
	}

	if (parameter > Capabilities::GetMaxTextureMaxAnisotropy()) {
		throw std::invalid_argument("Anisotropy parameter must be at most GL_MAX_TEXTURE_MAX_ANISOTROPY");
	}

	Bind();
	glTexParameterf(GLenum(_target), GL_TEXTURE_MAX_ANISOTROPY_EXT, parameter);
}

void Texture::GenerateMipmap() {
	Bind();
	glGenerateMipmap(GLenum(_target));
}

}
