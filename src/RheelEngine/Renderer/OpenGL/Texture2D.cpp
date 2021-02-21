/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Texture2D.h"

namespace rheel::gl {

Texture2D::Texture2D() :
		AbstractTexture(Target::TEXTURE_2D) {}

void Texture2D::SetWrapParameterS(WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLint(parameter));
}

void Texture2D::SetWrapParameterT(WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLint(parameter));
}

void Texture2D::SetCompareFunction(CompareFunction parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GLint(parameter));
}

void Texture2D::SetCompareMode(CompareMode parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GLint(parameter));
}

void Texture2D::SetEmpty(InternalFormat internal_format, unsigned width, unsigned height, Format format) {
	std::vector<float> zeros;
	zeros.resize(width * height * 4);
	SetData(internal_format, width, height, format, zeros.data());
}

void Texture2D::SetData(InternalFormat internal_format, unsigned width, unsigned height, Format format, Type type, const void* data) {
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GLenum(internal_format), width, height, 0, GLenum(format), GLenum(type), data);
}

void Texture2D::SetData(InternalFormat internal_format, unsigned width, unsigned height, Format format, const float* data) {
	SetData(internal_format, width, height, format, Type::FLOAT, data);
}

}