/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_HANDLE_H
#define RHEELENGINE_GL_HANDLE_H
#include "../../_common.h"

#include <GL/glew.h>

#define OPENGL_GENDEL_FUNCTION(func, name)          \
struct RE_API name {                                \
	void operator()(GLsizei n, GLuint *ptr) const { \
		func(n, ptr);                               \
	}                                               \
};

namespace rheel::GL {

template<typename Generator, typename Deleter>
class RE_API Handle {
	RE_NO_COPY(Handle)

public:
	/**
	 * Generates a handle with the generator.
	 */
	Handle() :
			_name(0) {

		_generator(1, &_name);
	}

	/**
	 * Uses the raw OpenGL handle as id.
	 */
	explicit Handle(GLuint raw) :
			_name(raw) {}

	/**
	 * Deletes the handle using the deleter.
	 */
	~Handle() {
		if (_name != 0) {
			_deleter(1, &_name);
		}
	}

	/**
	 * Moves the handle to a new object, and invalidates the original handle.
	 */
	Handle(Handle&& h) noexcept :
			_generator(std::move(h._generator)),
			_deleter(std::move(h._deleter)),
			_name(h._name) {

		// invalidate the original handle
		h._name = 0;
	}

	Handle& operator=(Handle&& h) noexcept {
		// delete the current handle
		if (_name != 0) {
			_deleter(1, &_name);
		}

		_generator = std::move(h._generator);
		_deleter = std::move(h._deleter);
		_name = h._name;

		// invalidate the original handle
		h._name = 0;

		return *this;
	}

	/**
	 * Returns the OpenGL name of this handle.
	 */
	GLuint GetName() const {
		return _name;
	}

private:
	Generator _generator;
	Deleter _deleter;

	GLuint _name;

};

}

#endif
