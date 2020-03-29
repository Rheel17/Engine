/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Program.h"

#include "State.h"

namespace rheel::GL {

Program::Program(Program&& p) noexcept :
		Object(std::forward<Object&&>(p)),
		_attached_shaders(std::move(p._attached_shaders)),
		_uniforms(std::move(p._uniforms)),
		_linked(p._linked) {}

Program& Program::operator=(Program&& p) noexcept {
	Object::operator=(std::forward<Object&&>(p));

	_attached_shaders = std::move(p._attached_shaders);
	_uniforms = std::move(p._uniforms);
	_linked = p._linked;
	return *this;
}

void Program::Use() const {
	_EnsureLinked();
	State::UseProgram(*this);
}

GLuint Program::GetHandle() const {
	return GetName();
}

void Program::AttachShader(Shader::ShaderType type, const std::string& source) {
	AttachShader(Shader(type, source));
}

void Program::AttachShader(const Shader& shader) {
	_EnsureNotLinked();
	glAttachShader(GetHandle(), shader.GetHandle());
	_attached_shaders.push_back(shader.GetHandle());
}

void Program::Link() {
	_EnsureNotLinked();

	glLinkProgram(GetHandle());

	// check link status
	GLint linked;
	glGetProgramiv(GetHandle(), GL_LINK_STATUS, &linked);

	if (!linked) {
		// get compile log (errors)
		GLint logSize;
		glGetProgramiv(GetHandle(), GL_INFO_LOG_LENGTH, &logSize);

		GLchar log[logSize];
		glGetProgramInfoLog(GetHandle(), logSize, &logSize, log);

		// throw with compile errors
		Log::Error() << "Failed to link shader program:\n" + std::string(log) << std::endl;
		abort();
	}

	// detach shaders
	for (GLuint shader : _attached_shaders) {
		glDetachShader(GetHandle(), shader);
	}

	_linked = true;
}

Uniform& Program::GetUniform(const std::string& name) const {
	return _GetUniform(name, true);
}

Uniform& Program::operator[](const std::string& name) const {
	return GetUniform(name);
}

bool Program::HasUniform(const std::string& name) const {
	return _GetUniform(name, false).IsValid();
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

Uniform& Program::_GetUniform(const std::string& name, bool checkWarning) const {
	auto iter = _uniforms.find(name);
	if (iter == _uniforms.end()) {
		iter = _uniforms.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(GetHandle(), name, checkWarning)).first;
	}

	return iter->second;
}

}