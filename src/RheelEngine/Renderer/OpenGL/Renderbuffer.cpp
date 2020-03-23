/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Renderbuffer.h"

#include "State.h"

namespace rheel::GL {

void Renderbuffer::Bind() const {
	State::BindRenderbuffer(*this);
}

void Renderbuffer::SetStorage(InternalFormat internalFormat, unsigned width, unsigned height) {
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GLenum(internalFormat), width, height);
}

void Renderbuffer::SetStorageMultisample(InternalFormat internalFormat, unsigned width, unsigned height, unsigned samples) {
	Bind();
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GLenum(internalFormat), width, height);
}

}