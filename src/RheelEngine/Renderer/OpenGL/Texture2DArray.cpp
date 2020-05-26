/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Texture2DArray.h"

namespace rheel::gl {

Texture2DArray::Texture2DArray() :
		AbstractTexture(Target::TEXTURE_2D_ARRAY) {}

void Texture2DArray::SetWrapParameterS(WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GLint(parameter));
}

void Texture2DArray::SetWrapParameterT(WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GLint(parameter));
}

void Texture2DArray::Initialize(InternalFormat internalFormat, unsigned int width, unsigned int height, unsigned int layers) {
	Bind();
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GLenum(internalFormat), width, height, layers);
}

void Texture2DArray::SetData(InternalFormat internalFormat, unsigned width, unsigned height, unsigned layers, Format format, Type type, const void* data) {
	Bind();
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GLenum(internalFormat), width, height, layers, 0, GLenum(format), GLenum(type), data);
}

void Texture2DArray::SetData(InternalFormat internalFormat, unsigned width, unsigned height, unsigned layers, Format format, const float* data) {
	SetData(internalFormat, width, height, layers, format, Type::FLOAT, data);
}

void Texture2DArray::SetLayerData(unsigned int width, unsigned int height, unsigned int layer, Format format, Type type, const void* data) {
	Bind();
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, width, height, 1, GLenum(format), GLenum(type), data);
}

void Texture2DArray::SetLayerData(unsigned int width, unsigned int height, unsigned int layer, Format format, const float* data) {
	SetLayerData(width, height, layer, format, Type::FLOAT, data);
}

}