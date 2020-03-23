/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_PROGRAM_H
#define RHEELENGINE_GL_PROGRAM_H
#include "../../_common.h"

#include "Shader.h"
#include "Uniform.h"

namespace rheel::GL {

class RE_API Program {
	RE_NO_COPY(Program);

public:
	Program();
	~Program();

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
	Uniform& GetUniform(const std::string& name);

	/**
	 * Returns a reference to the uniform of the specified name.
	 */
	Uniform& operator[](const std::string& name);

private:
	void _EnsureLinked() const;
	void _EnsureNotLinked() const;

	GLuint _handle;
	bool _linked = false;

	std::vector<GLuint> _attached_shaders;
	std::unordered_map<std::string, Uniform> _uniforms;

};

}

#endif
