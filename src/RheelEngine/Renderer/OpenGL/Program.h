/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_PROGRAM_H
#define RHEELENGINE_GL_PROGRAM_H
#include "../../_common.h"

#include "Object.h"
#include "Shader.h"
#include "Uniform.h"

namespace rheel::GL {

struct RE_API _CreateProgram {
	GLuint operator()() const {
		return glCreateProgram();
	}
};

struct RE_API _DeleteProgram {
	void operator()(GLuint name) const {
		glDeleteProgram(name);
	}
};

class RE_API Program : Object<_CreateProgram, _DeleteProgram> {

public:
	Program() = default;

	Program(Program&& p) noexcept;
	Program& operator=(Program&& p) noexcept;

	void Use() const;

	GLuint GetHandle() const;

	/**
	 * Creates a shader from the type and source, and attaches it to
	 * this program
	 */
	void AttachShader(Shader::ShaderType type, const std::string& source);

	/**
	 * Attaches a shader to the program.
	 */
	void AttachShader(const Shader& shader);

	/**
	 * Links the shaders together into this program.
	 */
	void Link();

	/**
	 * Returns a reference to the uniform of the specified name.
	 */
	Uniform& GetUniform(const std::string& name) const;

	/**
	 * Returns a reference to the uniform of the specified name.
	 */
	Uniform& operator[](const std::string& name) const;

	/**
	 * Returns true if the program has the uniform of the specified name.
	 */
	bool HasUniform(const std::string& name) const;

private:
	void _EnsureLinked() const;
	void _EnsureNotLinked() const;

	bool _linked = false;

	std::vector<GLuint> _attached_shaders;
	mutable std::unordered_map<std::string, Uniform> _uniforms;

};

}

#endif
