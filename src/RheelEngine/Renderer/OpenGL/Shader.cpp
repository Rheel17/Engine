/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Shader.h"

namespace rheel::gl {

Shader::Shader(ShaderType type, const std::string& source) :
		_type(type) {

	_handle = glCreateShader(GLenum(type));

	// add the shader source and compile
	const GLchar* str = source.c_str();
	const GLint len = source.size();

	glShaderSource(_handle, 1, &str, &len);
	glCompileShader(_handle);

	// check compile status
	GLint compiled = 0;
	glGetShaderiv(_handle, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint logSize = 0;
		glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<GLchar> log(logSize);
		glGetShaderInfoLog(_handle, logSize, &logSize, &log[0]);

		// throw with compile errors
		Log::Error() << "Failed to compile shader:\n" << std::string_view(log.begin(), log.end()) << std::endl;
		abort();
	}
}

Shader::~Shader() {
	if (_handle != 0) {
		glDeleteShader(_handle);
	}
}

Shader::Shader(Shader&& s) noexcept :
		_handle(s._handle),
		_type(s._type) {

	s._handle = 0;
}

Shader& Shader::operator=(Shader&& s) noexcept {
	// delete the current handle
	if (_handle != 0) {
		glDeleteShader(_handle);
	}

	// set new handle
	_type = s._type;
	_handle = s._handle;

	// invalidate the original handle
	s._handle = 0;

	return *this;
}

GLuint Shader::GetHandle() const {
	return _handle;
}

Shader::ShaderType Shader::GetType() const {
	return _type;
}

}