/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Renderbuffer.h"

#include "Context.h"

namespace rheel::gl {

void Renderbuffer::Bind() const {
	Context::Current().BindRenderbuffer(*this);
}

void Renderbuffer::SetStorage(InternalFormat internal_format, unsigned width, unsigned height) {
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GLenum(internal_format), width, height);
}

void Renderbuffer::SetStorageMultisample(InternalFormat internal_format, unsigned width, unsigned height, unsigned samples) {
	Bind();
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GLenum(internal_format), width, height);
}

}