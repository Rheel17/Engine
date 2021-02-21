/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "AbstractTexture.h"

#include "Capabilities.h"

namespace rheel::gl {

AbstractTexture::AbstractTexture(Texture::Target target) :
		Texture(target) {}

void AbstractTexture::SetMinifyingFilter(FilterFunction filter_function) {
	Bind();
	glTexParameteri(GLenum(GetTarget()), GL_TEXTURE_MIN_FILTER, GLint(filter_function));
}

void AbstractTexture::SetMagnificationFilter(FilterFunction filter_function) {
	if (filter_function != FilterFunction::NEAREST && filter_function != FilterFunction::LINEAR) {
		throw std::invalid_argument("Magnification filterFunction must be NEAREST of LINEAR");
	}

	Bind();
	glTexParameteri(GLenum(GetTarget()), GL_TEXTURE_MAG_FILTER, GLint(filter_function));
}

void AbstractTexture::SetAnisotropyParameter(float parameter) {
	if (parameter < 1.0f) {
		throw std::invalid_argument("Anisotropy parameter must be at least 1.0");
	}

	if (parameter > Capabilities::GetMaxTextureMaxAnisotropy()) {
		throw std::invalid_argument("Anisotropy parameter must be at most GL_MAX_TEXTURE_MAX_ANISOTROPY");
	}

	Bind();
	glTexParameterf(GLenum(GetTarget()), GL_TEXTURE_MAX_ANISOTROPY_EXT, parameter);
}

void AbstractTexture::GenerateMipmap() {
	Bind();
	glGenerateMipmap(GLenum(GetTarget()));
}

}