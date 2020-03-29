/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_HANDLE_H
#define RHEELENGINE_GL_HANDLE_H
#include "../../_common.h"

#include <GL/glew.h>

#define OPENGL_GEN_FUNCTION(func, structName)    \
struct RE_API structName {                       \
	static constexpr auto glfn = #func;			 \
	GLuint operator()() const {                  \
		GLuint name;							 \
		func(1, &name);                          \
		return name;                             \
	}                                            \
}

#define OPENGL_DELETE_FUNCTION(func, structName) \
struct RE_API structName {                       \
	static constexpr auto glfn = #func;			 \
	void operator()(GLuint name) const {         \
		func(1, &name);                          \
	}                                            \
}

namespace rheel::GL {

template<typename Generator, typename Deleter>
class RE_API Handle {
	RE_NO_COPY(Handle)

public:
	/**
	 * The real name is generated lazily - when it is needed.
	 */
	Handle() = default;

	/**
	 * Uses the raw OpenGL handle as id.
	 */
	explicit Handle(GLuint raw) :
			_name(raw), _generated(true) {}

	/**
	 * Deletes the handle using the deleter.
	 */
	~Handle() {
		if (_name != 0) {
			_deleter(_name);
		}
	}

	/**
	 * Moves the handle to a new object, and invalidates the original handle.
	 */
	Handle(Handle&& h) noexcept :
			_generator(std::move(h._generator)),
			_deleter(std::move(h._deleter)),
			_generated(h._generated),
			_name(h._name) {

		// invalidate the original handle
		h._name = 0;
	}

	Handle& operator=(Handle&& h) noexcept {
		// delete the current handle
		if (_name != 0) {
			_deleter(_name);
		}

		_generator = std::move(h._generator);
		_deleter = std::move(h._deleter);
		_generated = h._generated;
		_name = h._name;

		// invalidate the original handle
		h._generated = true;
		h._name = 0;

		return *this;
	}

	/**
	 * Returns the OpenGL name of this handle.
	 */
	GLuint GetName() const {
		if (!_generated) {
			_name = _generator();
			_generated = true;
		}

		return _name;
	}

private:
	Generator _generator;
	Deleter _deleter;

	mutable bool _generated = false;
	mutable GLuint _name = 0;

};

}

#endif
