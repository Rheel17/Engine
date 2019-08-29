#include "GLTexture2D.h"

namespace rheel {

GLTexture2D::GLTexture2D(GLuint width, GLuint height, GLuint internalFormat) :
		_width(width), _height(height), _internal_format(internalFormat) {

	_id = GL::GenTexture();
}

GLuint GLTexture2D::ID() const {
	return _id;
}

void GLTexture2D::Bind(GLuint textureUnit) const {
	GL::BindTexture(_id, GL::TextureTarget::TEXTURE_2D, textureUnit);
}

void GLTexture2D::SetMinifyingFilter(GL::FilterFunction filterFunction) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLint(filterFunction));
}

void GLTexture2D::SetMagnificationFilter(GL::FilterFunction filterFunction) {
	if (filterFunction != GL::FilterFunction::NEAREST && filterFunction != GL::FilterFunction::LINEAR) {
		throw std::invalid_argument("filterFunction must be NEAREST of LINEAR");
	}

	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLint(filterFunction));
}

void GLTexture2D::SetWrapParameterS(GL::WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLint(parameter));
}

void GLTexture2D::SetWrapParameterT(GL::WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLint(parameter));
}

void GLTexture2D::InitializeEmpty(GLenum format) {
	std::vector<GLubyte> zeroBytes(_width * _height * 16, 0);
	SetData(format, GL_UNSIGNED_BYTE, zeroBytes.data());
}

void GLTexture2D::SetData(GLenum format, GLenum type, const void *data) {
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, _width, _height, 0, format, type, data);

	_has_initialized = true;
}

void GLTexture2D::SetPartialData(int x, int y, unsigned width, unsigned height, GLenum format, GLenum type, const void *data) {
	if (!_has_initialized) {
		InitializeEmpty(format);
	}

	Bind();
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, format, type, data);
}

}
