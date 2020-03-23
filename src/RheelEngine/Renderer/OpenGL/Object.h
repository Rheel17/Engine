/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_OBJECT_H
#define RHEELENGINE_GL_OBJECT_H
#include "../../_common.h"

#include "Handle.h"

namespace rheel::GL {

template<typename Generator, typename Deleter>
class RE_API Object {

public:
	GLuint GetName() const {
		return _handle.GetName();
	}

private:
	Handle<Generator, Deleter> _handle;

};

}

#endif
