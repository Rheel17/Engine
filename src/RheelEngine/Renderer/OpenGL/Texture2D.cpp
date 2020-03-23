/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Texture2D.h"

namespace rheel::GL {

Texture2D::Texture2D() :
Texture(Target::TEXTURE_2D) {}

void Texture2D::SetWrapParameterS(Texture::WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLint(parameter));
}

void Texture2D::SetWrapParameterT(Texture::WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLint(parameter));
}

void Texture2D::SetEmpty(InternalFormat internalFormat, unsigned width, unsigned height) {
	std::vector<float> zeros;
	zeros.resize(width * height * 4);
	SetData(internalFormat, width, height, Format::RGBA, zeros.data());
}

void Texture2D::SetData(InternalFormat internalFormat, unsigned width, unsigned height, Format format, Type type, const void *data) {
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GLenum(internalFormat), width, height, 0, GLenum(format), GLenum(type), data);
}

void Texture2D::SetData(InternalFormat internalFormat, unsigned width, unsigned height, Format format, const float *data) {
	SetData(internalFormat, width, height, format, Type::FLOAT, data);
}

}