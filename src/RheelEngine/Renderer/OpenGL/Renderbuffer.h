/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_RENDERBUFFER_H
#define RHEELENGINE_GL_RENDERBUFFER_H
#include "../../_common.h"

#include "Object.h"
#include "Enums.h"

namespace rheel::GL {

OPENGL_GEN_FUNCTION(glGenRenderbuffers, _GenRenderbuffers);
OPENGL_DELETE_FUNCTION(glDeleteRenderbuffers, _DeleteRenderbuffers);

class RE_API Renderbuffer : public Object<_GenRenderbuffers, _DeleteRenderbuffers> {

public:
	Renderbuffer() = default;

	void Bind() const;

	void SetStorage(InternalFormat internalFormat, unsigned width, unsigned height);

	void SetStorageMultisample(InternalFormat internalFormat, unsigned width, unsigned height, unsigned samples);

};

}

#endif
