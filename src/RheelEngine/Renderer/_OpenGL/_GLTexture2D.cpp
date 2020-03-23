/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "_GLTexture2D.h"

namespace rheel {

_GLTexture2D::_GLTexture2D(GLuint width, GLuint height, GLuint internalFormat) :
		_width(width), _height(height), _internal_format(internalFormat) {

	_id = _GL::GenTexture();
}

GLuint _GLTexture2D::ID() const {
	return _id;
}

void _GLTexture2D::Bind(GLuint textureUnit) const {
	_GL::BindTexture(_id, _GL::TextureTarget::TEXTURE_2D, textureUnit);
}

void _GLTexture2D::SetMinifyingFilter(_GL::FilterFunction filterFunction) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLint(filterFunction));
}

void _GLTexture2D::SetMagnificationFilter(_GL::FilterFunction filterFunction) {
	if (filterFunction != _GL::FilterFunction::NEAREST && filterFunction != _GL::FilterFunction::LINEAR) {
		throw std::invalid_argument("filterFunction must be NEAREST of LINEAR");
	}

	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLint(filterFunction));
}

void _GLTexture2D::SetWrapParameterS(_GL::WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLint(parameter));
}

void _GLTexture2D::SetWrapParameterT(_GL::WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLint(parameter));
}

void _GLTexture2D::SetAnisotropyParameter(float parameter) {
	Bind();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, parameter);
}

void _GLTexture2D::GenerateMipmap() {
	Bind();
	glGenerateMipmap(GL_TEXTURE_2D);
}

void _GLTexture2D::InitializeEmpty(GLenum format) {
	std::vector<GLubyte> zeroBytes(_width * _height * 16, 0);
	SetData(format, GL_UNSIGNED_BYTE, zeroBytes.data());
}

void _GLTexture2D::SetData(GLenum format, GLenum type, const void *data) {
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, _width, _height, 0, format, type, data);

	_has_initialized = true;
}

void _GLTexture2D::SetPartialData(int x, int y, unsigned width, unsigned height, GLenum format, GLenum type, const void *data) {
	if (!_has_initialized) {
		InitializeEmpty(format);
	}

	Bind();
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, format, type, data);
}

}
