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

void GLTexture2DMultisample::Initialize(GLint internalFormat) {
	Bind();
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, internalFormat, _width, _height, true);
}

}
