/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Program.h"

#include "State.h"

namespace rheel::GL {

Program::Program() {
	_handle = glCreateProgram();
}

Program::~Program() {
	if (_handle != 0) {
		glDeleteProgram(_handle);
	}
}

Program::Program(Program&& p) noexcept :
		_handle(p._handle),
		_attached_shaders(std::move(p._attached_shaders)),
		_uniforms(std::move(p._uniforms)) {

	p._handle = 0;
}

Program& Program::operator=(Program&& p) noexcept {
	// delete the current handle
	if (_handle != 0) {
		glDeleteProgram(_handle);
	}

	// set new handle
	_handle = p._handle;

	_attached_shaders = std::move(p._attached_shaders);
	_uniforms = std::move(p._uniforms);

	// invalidate the original handle
	p._handle = 0;

	return *this;
}

void Program::Use() const {
	_EnsureLinked();
	State::UseProgram(*this);
}

GLuint Program::GetHandle() const {
	return _handle;
}

void Program::AttachShader(Shader::ShaderType type, const std::string& source) {
	AttachShader(Shader(type, source));
}

void Program::AttachShader(const Shader& shader) {
	_EnsureNotLinked();
	glAttachShader(_handle, shader.GetHandle());
	_attached_shaders.push_back(shader.GetHandle());
}

void Program::Link() {
	_EnsureNotLinked();

	glLinkProgram(_handle);

	// check link status
	GLint linked;
	glGetProgramiv(_handle, GL_LINK_STATUS, &linked);

	if (!linked) {
		// get compile log (errors)
		GLint logSize;
		glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &logSize);

		GLchar log[logSize];
		glGetProgramInfoLog(_handle, logSize, &logSize, log);

		// throw with compile errors
		Log::Error() << "Failed to link shader program:\n" + std::string(log) << std::endl;
		abort();
	}

	// detach shaders
	for (GLuint shader : _attached_shaders) {
		glDetachShader(_handle, shader);
	}
}

Uniform& Program::GetUniform(const std::string& name) {
	auto iter = _uniforms.find(name);
	if (iter == _uniforms.end()) {
		iter = _uniforms.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(_handle, name)).first;
	}

	return iter->second;
}

Uniform& Program::operator[](const std::string& name) {
	return GetUniform(name);
}

void Program::_EnsureLinked() const {
	if (!_linked) {
		Log::Error() << "Shader not linked" << std::endl;
		abort();
	}
}

void Program::_EnsureNotLinked() const {
	if (_linked) {
		Log::Error() << "Operation not allowed on a linked shader" << std::endl;
		abort();
	}
}

}