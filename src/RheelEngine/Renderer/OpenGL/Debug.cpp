/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Debug.h"

#include <regex>

namespace rheel::GL {

void callbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	std::string msg(message, length);
	Debug::_callback(id, Debug::Source(source), Debug::Type(type), Debug::Severity(severity), msg);
}

Debug::Callback Debug::_callback = [](unsigned, Source, Type, Severity, const std::string&){};
bool Debug::_initialized_opengl_callback = false;

std::string Debug::GetString(Source source) {
	switch (source) {
		case Source::API:             return "API";
		case Source::WINDOW_SYSTEM:   return "WINDOW_SYSTEM";
		case Source::SHADER_COMPILER: return "SHADER_COMPILER";
		case Source::THIRD_PARTY:     return "THIRD_PARTY";
		case Source::APPLICATION:     return "APPLICATION";
		case Source::OTHER:           return "OTHER";
		default: return "";
	}
}

std::string Debug::GetString(Type type) {
	switch (type) {
		case Type::ERROR:               return "ERROR";
		case Type::DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case Type::UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
		case Type::PORTABILITY:         return "PORTABILITY";
		case Type::PERFORMANCE:         return "PERFORMANCE";
		case Type::MARKER:              return "MARKER";
		case Type::PUSH_GROUP:          return "PUSH_GROUP";
		case Type::POP_GROUP:           return "POP_GROUP";
		case Type::OTHER:               return "OTHER";
		default: return "";
	}
}

std::string Debug::GetString(Severity severity) {
	switch (severity) {
		case Severity::HIGH:         return "HIGH";
		case Severity::MEDIUM:       return "MEDIUM";
		case Severity::LOW:          return "LOW";
		case Severity::NOTIFICATION: return "NOTIFICATION";
		default: return "";
	}
}

void Debug::SetDebugCallback(Callback callback) {
	_callback = std::move(callback);

	if (!_initialized_opengl_callback) {
		GLint debugFlags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &debugFlags);

		if (debugFlags & GL_CONTEXT_FLAG_DEBUG_BIT) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(callbackFunction, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}

		_initialized_opengl_callback = true;
	}
}

void Debug::ClearDebugCallback() {
	_callback = [](unsigned, Source, Type, Severity, const std::string&){};
}

}