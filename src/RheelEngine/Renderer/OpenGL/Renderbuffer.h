/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_RENDERBUFFER_H
#define RHEELENGINE_RENDERBUFFER_H
#include "../../_common.h"

#include "Object.h"
#include "Enums.h"

namespace rheel::gl {

OPENGL_GEN_FUNCTION(glGenRenderbuffers, gen_renderbuffers_);
OPENGL_DELETE_FUNCTION(glDeleteRenderbuffers, delete_renderbuffers_);

class RE_API Renderbuffer : public Object<gen_renderbuffers_, delete_renderbuffers_> {

public:
	Renderbuffer() = default;

	void Bind() const;

	void SetStorage(InternalFormat internal_format, unsigned width, unsigned height);

	void SetStorageMultisample(InternalFormat internal_format, unsigned width, unsigned height, unsigned samples);

};

}

#endif
