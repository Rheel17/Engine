/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_HANDLE_H
#define RHEELENGINE_HANDLE_H
#include "../../_common.h"

#include <GL/glwr.h>

#define OPENGL_GEN_FUNCTION(func, structName)    \
struct structName {                              \
    static constexpr auto glfn = #func;          \
    GLuint operator()() const {                  \
        GLuint name;                             \
        func(1, &name);                          \
        return name;                             \
    }                                            \
}

#define OPENGL_DELETE_FUNCTION(func, structName) \
struct structName {                              \
    static constexpr auto glfn = #func;          \
    void operator()(GLuint name) const {         \
        func(1, &name);                          \
    }                                            \
}

namespace rheel::gl {

template<typename Generator, typename Deleter>
class Handle {
	RE_NO_COPY(Handle);

public:
	/**
	 * Generate the handle.
	 */
	Handle() :
			_name(Generator{}()) {}

	/**
	 * Uses the raw OpenGL name as handle.
	 */
	explicit Handle(GLuint raw) :
			_name(raw) {}

	/**
	 * Deletes the handle using the deleter.
	 */
	~Handle() {
		if (_name != 0) {
			Deleter{}(_name);
		}
	}

	/**
	 * Moves the handle to a new object, and invalidates the original handle.
	 */
	Handle(Handle&& h) noexcept:
			_generated(h._generated),
			_name(h._name) {

		// invalidate the original handle
		h._name = 0;
	}

	Handle& operator=(Handle&& h) noexcept {
		// delete the current handle
		if (_name != 0) {
			Deleter{}(_name);
		}

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
		return _name;
	}

private:
	mutable bool _generated = false;
	mutable GLuint _name = 0;

};

}

#endif
