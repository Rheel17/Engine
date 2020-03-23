/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "_GLShaderProgram.h"

#include <iostream>
#include <fstream>

namespace rheel {

GLuint GLShaderProgramBindings::programInUse = 0;

void GLShaderProgramBindings::Use(GLuint program) {
	if (programInUse != program) {

#if RE_GL_DEBUG
		std::cout << "glUseProgram(" << program << ")" << std::endl;
#endif
		glUseProgram(program);
		programInUse = program;
	}
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-c-copy-assignment-signature"
#pragma ide diagnostic ignored "misc-unconventional-assign-operator"
void GLShaderUniform::operator=(GLfloat value) { _Use(); glUniform1f(_location, value); }
void GLShaderUniform::operator=(GLint value) { _Use(); glUniform1i(_location, value); }
void GLShaderUniform::operator=(GLuint value) { _Use(); glUniform1ui(_location, value); }
void GLShaderUniform::operator=(const uvec2& value) { _Use(); glUniform2ui(_location, value.x, value.y); }
void GLShaderUniform::operator=(const uvec3& value) { _Use(); glUniform3ui(_location, value.x, value.y, value.z); }
void GLShaderUniform::operator=(const uvec4& value) { _Use(); glUniform4ui(_location, value.x, value.y, value.z, value.w); }
void GLShaderUniform::operator=(const std::vector<GLfloat>& values) { _Use(); glUniform1fv(_location, values.size(), values.data()); }
void GLShaderUniform::operator=(const std::vector<GLint>& values) { _Use(); glUniform1iv(_location, values.size(), values.data()); }
void GLShaderUniform::operator=(const std::vector<GLuint>& values) { _Use(); glUniform1uiv(_location, values.size(), values.data()); }
void GLShaderUniform::operator=(const std::vector<uvec2>& values) { _Use(); glUniform2uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data())); }
void GLShaderUniform::operator=(const std::vector<uvec3>& values) { _Use(); glUniform3uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data())); }
void GLShaderUniform::operator=(const std::vector<uvec4>& values) { _Use(); glUniform4uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data())); }
void GLShaderUniform::operator=(const mat2x4& value) { _Use(); glUniformMatrix2x4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
void GLShaderUniform::operator=(const mat3x4& value) { _Use(); glUniformMatrix3x4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
void GLShaderUniform::operator=(const mat4x3& value) { _Use(); glUniformMatrix4x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
void GLShaderUniform::operator=(const std::vector<mat2x4>& value) { _Use(); glUniformMatrix2x4fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
void GLShaderUniform::operator=(const std::vector<mat3x4>& value) { _Use(); glUniformMatrix3x4fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
void GLShaderUniform::operator=(const std::vector<mat4x3>& value) { _Use(); glUniformMatrix4x3fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
void GLShaderUniform::operator=(const std::vector<mat4x2>& value) { _Use(); glUniformMatrix4x2fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
void GLShaderUniform::operator=(const std::vector<mat3x2>& value) { _Use(); glUniformMatrix3x2fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
void GLShaderUniform::operator=(const std::vector<mat2x3>& value) { _Use(); glUniformMatrix2x3fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
void GLShaderUniform::operator=(const std::vector<mat4>& value) { _Use();   glUniformMatrix4fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
void GLShaderUniform::operator=(const std::vector<mat3>& value) { _Use();   glUniformMatrix3fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
void GLShaderUniform::operator=(const std::vector<mat2>& value) { _Use();   glUniformMatrix2fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
void GLShaderUniform::operator=(const mat4x2& value) { _Use(); glUniformMatrix4x2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
void GLShaderUniform::operator=(const mat3x2& value) { _Use(); glUniformMatrix3x2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
void GLShaderUniform::operator=(const mat2x3& value) { _Use(); glUniformMatrix2x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
void GLShaderUniform::operator=(const mat4& value) { _Use();   glUniformMatrix4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
void GLShaderUniform::operator=(const mat3& value) { _Use();   glUniformMatrix3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
void GLShaderUniform::operator=(const mat2& value) { _Use();   glUniformMatrix2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
void GLShaderUniform::operator=(const std::vector<ivec4>& values) { _Use(); glUniform4iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data())); }
void GLShaderUniform::operator=(const std::vector<ivec3>& values) { _Use(); glUniform3iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data())); }
void GLShaderUniform::operator=(const std::vector<ivec2>& values) { _Use(); glUniform2iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data())); }
void GLShaderUniform::operator=(const std::vector<vec4>& values) { _Use(); glUniform4fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data())); }
void GLShaderUniform::operator=(const std::vector<vec3>& values) { _Use(); glUniform3fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data())); }
void GLShaderUniform::operator=(const std::vector<vec2>& values) { _Use(); glUniform2fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data())); }
void GLShaderUniform::operator=(const ivec4& value) { _Use(); glUniform4i(_location, value.x, value.y, value.z, value.w); }
void GLShaderUniform::operator=(const ivec3& value) { _Use(); glUniform3i(_location, value.x, value.y, value.z); }
void GLShaderUniform::operator=(const ivec2& value) { _Use(); glUniform2i(_location, value.x, value.y); }
void GLShaderUniform::operator=(const vec4& value) { _Use(); glUniform4f(_location, value.x, value.y, value.z, value.w); }
void GLShaderUniform::operator=(const vec3& value) { _Use(); glUniform3f(_location, value.x, value.y, value.z); }
void GLShaderUniform::operator=(const vec2& value) { _Use(); glUniform2f(_location, value.x, value.y); }
#pragma clang diagnostic pop

void _GLShaderProgram::AddShaderFromSource(ShaderType type, std::string source) {
	_EnsureNotLinked();
	_shaders_to_add[type] = std::make_pair(false, std::move(source));
}

void _GLShaderProgram::AddShaderFromFile(ShaderType type, std::string file) {
	_EnsureNotLinked();
	_shaders_to_add[type] = std::make_pair(true, std::move(file));
}

GLShaderUniform& _GLShaderProgram::operator[](const std::string& name) const {
	_EnsureLinked();
	_EnsureUniformInstance(name);
	return _uniforms[name];
}

GLShaderUniform& _GLShaderProgram::operator[](const char *name) const {
	_EnsureLinked();
	_EnsureUniformInstance(name);
	return _uniforms[name];
}

GLShaderUniform& _GLShaderProgram::GetUniform(const std::string& name) const {
	_EnsureLinked();
	_EnsureUniformInstance(name);
	return _uniforms[name];
}

GLShaderUniform& _GLShaderProgram::GetUniform(const char *name) const {
	_EnsureLinked();
	_EnsureUniformInstance(name);
	return _uniforms[name];
}

bool _GLShaderProgram::HasUniform(const std::string& name) const {
	// TODO: optimize by caching the uniform location of all names
	return glGetUniformLocation(_id, name.c_str()) >= 0;
}

bool _GLShaderProgram::HasUniform(const char *name) const {
	return glGetUniformLocation(_id, name) >= 0;
}

void _GLShaderProgram::Link() {
	_EnsureNotLinked();

	// create the _OpenGL program
	_id = _GL::CreateProgram();

	// actually adds the Shaders to the program
	std::vector<GLuint> shaders;

	for (const auto& addIterator : _shaders_to_add) {
		const auto& toAdd = addIterator.second;

		if (toAdd.first) { // toAdd.first is true if the shader is a file
			_AddShaderFromFile(addIterator.first, std::move(toAdd.second));
		} else { // else, it is the complete source
			_AddShaderFromSource(addIterator.first, std::move(toAdd.second));
		}
	}

	// remove the to-add Shaders, since now they only take up memory
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
		Log::Error() << "Failed to link shader program:\n" + std::string(log) << std::endl;
		throw std::runtime_error("Failed to link shader program:\n" + std::string(log));
	}

	// remove Shaders
	for (const GLuint& shader : shaders) {
		glDetachShader(_id, shader);
		glDeleteShader(shader);
	}

	_linked = true;
}

GLuint _GLShaderProgram::_AddShaderFromSource(GLuint type, std::string source) {
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
		Log::Error() << "Failed to compile shader:\n" << std::string(log) << std::endl;
		throw std::runtime_error("Failed to compile shader:\n" + std::string(log));
	}

	// add the shader to the program
	glAttachShader(_id, shader);
	return shader;
}

GLuint _GLShaderProgram::_AddShaderFromFile(GLuint type, std::string file) {
	std::ifstream f(file);

	if (!f) {
		throw std::runtime_error("Error while reading shader file: " + file);
	}

	std::string source((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

	return _AddShaderFromSource(type, std::move(source));
}

void _GLShaderProgram::_EnsureUniformInstance(const std::string& name) const {
	if (_uniforms.find(name) == _uniforms.end()) {
		_uniforms.emplace(name, GLShaderUniform(_id, _GetUniformLocation(name)));
	}
}

GLint _GLShaderProgram::_GetUniformLocation(const std::string& name) const {
	GLint location = glGetUniformLocation(_id, name.c_str());

	if (location < 0) {
		Log::Warning() << "shader doesn't have uniform " << name << std::endl;
	}

	return location;
}

void _GLShaderProgram::_EnsureLinked() const {
	if (!_linked) {
		throw std::runtime_error("Operation not available before the program is linked");
	}
}

void _GLShaderProgram::_EnsureNotLinked() const {
	if (_linked) {
		throw std::runtime_error("Operation not available after the program has been linked");
	}
}

}
