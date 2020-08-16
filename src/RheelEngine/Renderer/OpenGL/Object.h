/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_OBJECT_H
#define RHEELENGINE_OBJECT_H
#include "../../_common.h"

#include "Handle.h"

namespace rheel::gl {

template<typename Generator, typename Deleter>
class Object {

public:
	GLuint GetName() const {
		return _handle.GetName();
	}

protected:
	Object() = default;

	explicit Object(GLuint name) :
			_handle(name) {}

private:
	Handle<Generator, Deleter> _handle;

};

}

#endif
