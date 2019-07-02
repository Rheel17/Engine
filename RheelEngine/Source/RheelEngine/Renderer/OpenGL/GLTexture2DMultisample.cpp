#include "GLTexture2DMultisample.h"

namespace rheel {

GLTexture2DMultisample::GLTexture2DMultisample(GLuint width, GLuint height, GLuint samples) :
		_width(width), _height(height), _samples(samples) {

	_id = GL::GenTexture();
}

GLuint GLTexture2DMultisample::GetID() const {
	return _id;
}

void GLTexture2DMultisample::Bind(GLuint textureUnit) const {
	GL::BindTexture(_id, GL::TextureTarget::TEXTURE_2D_MULTISAMPLE, textureUnit);
}

void GLTexture2DMultisample::SetMinifyingFilter(GL::FilterFunction filterFunction) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GLint(filterFunction));
}

void GLTexture2DMultisample::SetMagnificationFilter(GL::FilterFunction filterFunction) {
	if (filterFunction != GL::FilterFunction::NEAREST && filterFunction != GL::FilterFunction::LINEAR) {
		throw std::invalid_argument("filterFunction must be NEAREST of LINEAR");
	}

	Bind();
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GLint(filterFunction));
}

void GLTexture2DMultisample::SetWrapParameterS(GL::WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GLint(parameter));
}

void GLTexture2DMultisample::SetWrapParameterT(GL::WrapParameter parameter) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GLint(parameter));
}

void GLTexture2DMultisample::Initialize(GLint internalFormat) {
	Bind();
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, internalFormat, _width, _height, true);
}

}
