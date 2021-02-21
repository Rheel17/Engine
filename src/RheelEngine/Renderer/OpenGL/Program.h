/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_PROGRAM_H
#define RHEELENGINE_PROGRAM_H
#include "../../_common.h"

#include "Object.h"
#include "Shader.h"
#include "Uniform.h"

namespace rheel::gl {

struct RE_API create_program {
	static constexpr auto glfn = "glCreateProgram";
	GLuint operator()() const {
		return glCreateProgram();
	}
};

struct RE_API delete_program {
	static constexpr auto glfn = "glDeleteProgram";
	void operator()(GLuint name) const {
		glDeleteProgram(name);
	}
};

class RE_API Program : Object<create_program, delete_program> {

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
	void _ensure_linked() const;
	void _ensure_not_linked() const;

	Uniform& _get_uniform(const std::string& name, bool check_warning) const;

	bool _linked = false;

	std::vector<GLuint> _attached_shaders;
	mutable std::unordered_map<std::string, Uniform> _uniforms;

};

}

#endif
