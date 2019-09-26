/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "GLShaderProgram.h"

#include <iostream>
#include <fstream>

namespace rheel {

GLuint GLShaderProgramBindings::programInUse = 0;

void GLShaderProgram::AddShaderFromSource(ShaderType type, std::string source) {
	_EnsureNotLinked();
	_shaders_to_add[type] = std::make_pair(false, std::move(source));
}

void GLShaderProgram::AddShaderFromFile(ShaderType type, std::string file) {
	_EnsureNotLinked();
	_shaders_to_add[type] = std::make_pair(true, std::move(file));
}

GLShaderUniform& GLShaderProgram::operator[](const std::string& name) const {
	_EnsureLinked();
	_EnsureUniformInstance(name);
	return _uniforms[name];
}

GLShaderUniform& GLShaderProgram::operator[](const char *name) const {
	_EnsureLinked();
	_EnsureUniformInstance(name);
	return _uniforms[name];
}

GLShaderUniform& GLShaderProgram::GetUniform(const std::string& name) const {
	_EnsureLinked();
	_EnsureUniformInstance(name);
	return _uniforms[name];
}

GLShaderUniform& GLShaderProgram::GetUniform(const char *name) const {
	_EnsureLinked();
	_EnsureUniformInstance(name);
	return _uniforms[name];
}

void GLShaderProgram::Link() {
	_EnsureNotLinked();

	// create the OpenGL program
	_id = GL::CreateProgram();

	// actually adds the shaders to the program
	std::vector<GLuint> shaders;

	for (const auto& addIterator : _shaders_to_add) {
		const auto& toAdd = addIterator.second;

		if (toAdd.first) { // toAdd.first is true if the shader is a file
			_AddShaderFromFile(addIterator.first, std::move(toAdd.second));
		} else { // else, it is the complete source
			_AddShaderFromSource(addIterator.first, std::move(toAdd.second));
		}
	}

	// remove the to-add shaders, since now they only take up memory
	_shaders_to_add.clear();

	// link the program
	glLinkProgram(_id);

	// check link status
	GLint linked;
	glGetProgramiv(_id, GL_LINK_STATUS, &linked);

	if (!linked) {
		// get compile log (errors)
		GLint logSize;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &logSize);

		GLchar log[logSize];
		glGetProgramInfoLog(_id, logSize, &logSize, log);

		// throw with compile errors
		std::cerr << "Failed to link shader program:\n" + std::string(log) << std::endl;
		throw std::runtime_error("Failed to link shader program:\n" + std::string(log));
	}

	// remove shaders
	for (const GLuint& shader : shaders) {
		glDetachShader(_id, shader);
		glDeleteShader(shader);
	}

	_linked = true;
}

GLuint GLShaderProgram::_AddShaderFromSource(GLuint type, std::string source) {
	// create the shader object
	GLuint shader = glCreateShader(type);
	if (shader == 0) {
		throw std::runtime_error("Failed to create shader");
	}

	// add the shader source and compile
	const GLchar *str = source.c_str();
	const GLint len = source.size();

	glShaderSource(shader, 1, &str, &len);
	glCompileShader(shader);

	// check compile status
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		// get compile log (errors)
		GLint logSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		GLchar log[logSize];
		glGetShaderInfoLog(shader, logSize, &logSize, log);

		// throw with compile errors
		std::cerr << "Failed to compile shader:\n" << std::string(log) << std::endl;
		throw std::runtime_error("Failed to compile shader:\n" + std::string(log));
	}

	// add the shader to the program
	glAttachShader(_id, shader);
	return shader;
}

GLuint GLShaderProgram::_AddShaderFromFile(GLuint type, std::string file) {
	std::ifstream f(file);

	if (!f) {
		throw std::runtime_error("Error while reading shader file: " + file);
	}

	std::string source((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

	return _AddShaderFromSource(type, std::move(source));
}

void GLShaderProgram::_EnsureUniformInstance(const std::string& name) const {
	if (_uniforms.find(name) == _uniforms.end()) {
		_uniforms.emplace(name, GLShaderUniform(_id, _GetUniformLocation(name)));
	}
}

GLint GLShaderProgram::_GetUniformLocation(const std::string& name) const {
	GLint location = glGetUniformLocation(_id, name.c_str());

	if (location < 0) {
		std::cerr << "shader doesn't have uniform " << name << std::endl;
	}

	return location;
}

void GLShaderProgram::_EnsureLinked() const {
	if (!_linked) {
		throw std::runtime_error("Operation not available before the program is linked");
	}
}

void GLShaderProgram::_EnsureNotLinked() const {
	if (_linked) {
		throw std::runtime_error("Operation not available after the program has been linked");
	}
}

}
