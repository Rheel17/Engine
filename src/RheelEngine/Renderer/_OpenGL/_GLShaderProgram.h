/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef GLSHADERPROGRAM_H_
#define GLSHADERPROGRAM_H_
#include "../../_common.h"

#if RE_GL_DEBUG
#include <iostream>
#endif

#include "_GL.h"

namespace rheel {

class RE_API GLShaderProgramBindings {
	friend class GLShaderUniform;
	friend class _GLShaderProgram;

public:
	GLShaderProgramBindings() = delete;

private:
	static void Use(GLuint program);

	static GLuint programInUse;

};

class RE_API GLShaderUniform {

public:
	explicit GLShaderUniform(GLuint program = 0, GLuint location = -1) : _program(program), _location(location) {}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-c-copy-assignment-signature"
#pragma ide diagnostic ignored "misc-unconventional-assign-operator"

	void operator=(GLfloat value);
	void operator=(const vec2& value);
	void operator=(const vec3& value);
	void operator=(const vec4& value);

	void operator=(GLint value);
	void operator=(const ivec2& value);
	void operator=(const ivec3& value);
	void operator=(const ivec4& value);

	void operator=(GLuint value);
	void operator=(const uvec2& value);
	void operator=(const uvec3& value);
	void operator=(const uvec4& value);

	void operator=(const std::vector<GLfloat>& values);
	void operator=(const std::vector<vec2>& values);
	void operator=(const std::vector<vec3>& values);
	void operator=(const std::vector<vec4>& values);

	void operator=(const std::vector<GLint>& values);
	void operator=(const std::vector<ivec2>& values);
	void operator=(const std::vector<ivec3>& values);
	void operator=(const std::vector<ivec4>& values);

	void operator=(const std::vector<GLuint>& values);
	void operator=(const std::vector<uvec2>& values);
	void operator=(const std::vector<uvec3>& values);
	void operator=(const std::vector<uvec4>& values);

	void operator=(const mat2& value);
	void operator=(const mat3& value);
	void operator=(const mat4& value);
	void operator=(const mat2x3& value);
	void operator=(const mat3x2& value);
	void operator=(const mat2x4& value);
	void operator=(const mat4x2& value);
	void operator=(const mat3x4& value);
	void operator=(const mat4x3& value);

	void operator=(const std::vector<mat2>& value);
	void operator=(const std::vector<mat3>& value);
	void operator=(const std::vector<mat4>& value);
	void operator=(const std::vector<mat2x3>& value);
	void operator=(const std::vector<mat3x2>& value);
	void operator=(const std::vector<mat2x4>& value);
	void operator=(const std::vector<mat4x2>& value);
	void operator=(const std::vector<mat3x4>& value);
	void operator=(const std::vector<mat4x3>& value);

#pragma clang diagnostic pop

private:
	GLuint _program;
	GLint _location;

	void _Use() { GLShaderProgramBindings::Use(_program); }

};

/**
 * Use this class for a high-level representation of an _OpenGL shader. After
 * creating an instance, add shader source by calling AddShaderFromSource(...)
 * or AddShaderFromFile(...). Before the first use, call Link() to actually
 * compile and link the Shaders. The function Use() should be used to use this
 * shader program. The operator [] with string arguments can be used to set
 * uniform variables.
 */
class RE_API _GLShaderProgram {

public:
	enum ShaderType {
		VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER
	};

public:
	/**
	 * Adds a shader from source code, given as a string. Multiple calls with
	 * the same type will only add the last-added shader to the program. Calling
	 * this is only valid before Link() is called.
	 */
	void AddShaderFromSource(ShaderType type, std::string source);

	/**
	 * Adds a shader from a source file. Multiple calls with the same type will
	 * only add the last-added shader to the program. Note that the file will be
	 * read when the shader is linked, not at the call of this method.
	 */
	void AddShaderFromFile(ShaderType type, std::string file);

	/**
	 * Compiles the added Shaders, and links them together.
	 */
	void Link();

	/**
	 * Use this shader program.
	 */
	inline void Use() const { GLShaderProgramBindings::Use((GLuint) _id); }

	/**
	 * Clears the use of any shader program
	 */
	static void ClearUse() { GLShaderProgramBindings::Use(0); }

	/**
	 * This operator can be used as a shorthand for setting uniform variables.
	 * Note that the shader will be in use after A uniform is set, but not
	 * necessarily after this operator call.
	 */
	GLShaderUniform& operator[](const std::string& name) const;

	/**
	 * This operator can be used as a shorthand for setting uniform variables.
	 * Note that the shader will be in use after A uniform is set, but not
	 * necessarily after this operator call.
	 */
	GLShaderUniform& operator[](const char *name) const;

	/**
	 * This method can be used for setting uniform variables. Note that the
	 * shader will be in use after A uniform is set, but not necessarily after
	 * this method call.
	 */
	GLShaderUniform& GetUniform(const std::string& name) const;

	/**
	 * This method can be used for setting uniform variables. Note that the
	 * shader will be in use after A uniform is set, but not necessarily after
	 * this method call.
	 */
	GLShaderUniform& GetUniform(const char *name) const;

	/**
	 * Use this method to find out whether a shader has a uniform
	 */
	 bool HasUniform(const std::string& name) const;

	/**
	 * Use this method to find out whether a shader has a uniform
	 */
	 bool HasUniform(const char *name) const;

private:
	GLuint _AddShaderFromSource(GLuint type, std::string source);
	GLuint _AddShaderFromFile(GLuint type, std::string file);

	void _EnsureUniformInstance(const std::string& name) const;
	GLint _GetUniformLocation(const std::string& name) const;

	void _EnsureLinked() const;
	void _EnsureNotLinked() const;

	GLhandle _id;
	bool _linked = false;

	std::unordered_map<ShaderType, std::pair<bool, std::string>> _shaders_to_add;

	mutable std::unordered_map<std::string, GLShaderUniform> _uniforms;

};

}

#endif
