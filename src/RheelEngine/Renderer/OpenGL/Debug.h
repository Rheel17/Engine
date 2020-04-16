/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_DEBUG_H
#define RHEELENGINE_DEBUG_H
#include "../../_common.h"

#include <gl/glew.h>

namespace rheel::gl {

class RE_API Debug {

public:
	RE_NO_CONSTRUCT(Debug);

public:
	enum class Source {
		API             = GL_DEBUG_SOURCE_API,
		WINDOW_SYSTEM   = GL_DEBUG_SOURCE_WINDOW_SYSTEM,
		SHADER_COMPILER = GL_DEBUG_SOURCE_SHADER_COMPILER,
		THIRD_PARTY     = GL_DEBUG_SOURCE_THIRD_PARTY,
		APPLICATION     = GL_DEBUG_SOURCE_APPLICATION,
		OTHER           = GL_DEBUG_SOURCE_OTHER
	};

	enum class Type {
		ERROR               = GL_DEBUG_TYPE_ERROR,
		DEPRECATED_BEHAVIOR = GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
		UNDEFINED_BEHAVIOR  = GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
		PORTABILITY         = GL_DEBUG_TYPE_PORTABILITY,
		PERFORMANCE         = GL_DEBUG_TYPE_PERFORMANCE,
		MARKER              = GL_DEBUG_TYPE_MARKER,
		PUSH_GROUP          = GL_DEBUG_TYPE_PUSH_GROUP,
		POP_GROUP           = GL_DEBUG_TYPE_POP_GROUP,
		OTHER               = GL_DEBUG_TYPE_OTHER
	};

	enum class Severity {
		HIGH         = GL_DEBUG_SEVERITY_HIGH,
		MEDIUM       = GL_DEBUG_SEVERITY_MEDIUM,
		LOW          = GL_DEBUG_SEVERITY_LOW,
		NOTIFICATION = GL_DEBUG_SEVERITY_NOTIFICATION
	};

	// void(unsigned id, Source source, Type type, Severity severity, const std::string& message)
	using Callback = std::function<void(unsigned, Source, Type, Severity, const std::string&)>;

public:
	static std::string GetString(Source source);
	static std::string GetString(Type type);
	static std::string GetString(Severity severity);

	static void SetDebugCallback(Callback callback);
	static void ClearDebugCallback();

private:
	friend void callbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void *userParam);

	static Callback _callback;
	static bool _initialized_opengl_callback;

};

}

#endif
