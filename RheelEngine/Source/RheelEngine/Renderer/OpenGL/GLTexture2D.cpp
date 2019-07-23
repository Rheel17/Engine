#include "GLTexture2D.h"

namespace rheel {

GLTexture2D::GLTexture2D(GLuint width, GLuint height) :
		_width(width), _height(height) {

	_id = GL::GenTexture();
}

GLuint GLTexture2D::GetID() const {
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

void GLTexture2D::SetData(GLint internalFormat, GLenum format, GLenum type, const void *data) {
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, type, data);

	_has_set_data = true;
}

void GLTexture2D::SetPartialData(int x, int y, unsigned width, unsigned height, GLenum format, GLenum type, const void *data) {
	if (!_has_set_data) {
		throw std::runtime_error("SetPartialData() was called without a prior call to SetData().");
	}

	std::cout << static_cast<const float *>(data)[width * height - 1] << std::endl;

	Bind();
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, format, type, data);
}

}
