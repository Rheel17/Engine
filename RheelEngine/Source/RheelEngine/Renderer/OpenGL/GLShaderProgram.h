#ifndef GLSHADERPROGRAM_H_
#define GLSHADERPROGRAM_H_
#include "../../_common.h"

#if RE_GL_DEBUG
#include <iostream>
#endif

#include "GL.h"

namespace rheel {

class RE_API GLShaderProgramBindings {
	friend class GLShaderUniform;
	friend class GLShaderProgram;

	GLShaderProgramBindings() = delete;

private:
	inline static void Use(GLuint program) {
		if (programInUse != program) {

#if RE_GL_DEBUG
			std::cout << "glUseProgram(" << program << ")" << std::endl;
#endif
			glUseProgram(program);
			programInUse = program;
		}
	}

	static GLuint programInUse;

};

class RE_API GLShaderUniform {

public:
	GLShaderUniform(GLuint program = 0, GLuint location = -1) : _program(program), _location(location) {}

	inline void operator=(GLfloat value)     { _Use(); glUniform1f(_location, value); }
	inline void operator=(const vec2& value) { _Use(); glUniform2f(_location, value.x, value.y); }
	inline void operator=(const vec3& value) { _Use(); glUniform3f(_location, value.x, value.y, value.z); }
	inline void operator=(const vec4& value) { _Use(); glUniform4f(_location, value.x, value.y, value.z, value.w); }

	inline void operator=(GLint value)        { _Use(); glUniform1i(_location, value); }
	inline void operator=(const ivec2& value) { _Use(); glUniform2i(_location, value.x, value.y); }
	inline void operator=(const ivec3& value) { _Use(); glUniform3i(_location, value.x, value.y, value.z); }
	inline void operator=(const ivec4& value) { _Use(); glUniform4i(_location, value.x, value.y, value.z, value.w); }

	inline void operator=(GLuint value)       { _Use(); glUniform1ui(_location, value); }
	inline void operator=(const uvec2& value) { _Use(); glUniform2ui(_location, value.x, value.y); }
	inline void operator=(const uvec3& value) { _Use(); glUniform3ui(_location, value.x, value.y, value.z); }
	inline void operator=(const uvec4& value) { _Use(); glUniform4ui(_location, value.x, value.y, value.z, value.w); }

	inline void operator=(const std::vector<GLfloat>& values) { _Use(); glUniform1fv(_location, values.size(), values.data()); }
	inline void operator=(const std::vector<vec2>& values)    { _Use(); glUniform2fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data())); }
	inline void operator=(const std::vector<vec3>& values)    { _Use(); glUniform3fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data())); }
	inline void operator=(const std::vector<vec4>& values)    { _Use(); glUniform4fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data())); }

	inline void operator=(const std::vector<GLint>& values) { _Use(); glUniform1iv(_location, values.size(), values.data()); }
	inline void operator=(const std::vector<ivec2>& values) { _Use(); glUniform2iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data())); }
	inline void operator=(const std::vector<ivec3>& values) { _Use(); glUniform3iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data())); }
	inline void operator=(const std::vector<ivec4>& values) { _Use(); glUniform4iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data())); }

	inline void operator=(const std::vector<GLuint>& values) { _Use(); glUniform1uiv(_location, values.size(), values.data()); }
	inline void operator=(const std::vector<uvec2>& values)  { _Use(); glUniform2uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data())); }
	inline void operator=(const std::vector<uvec3>& values)  { _Use(); glUniform3uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data())); }
	inline void operator=(const std::vector<uvec4>& values)  { _Use(); glUniform4uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data())); }

	inline void operator=(const mat2& value)   { _Use();   glUniformMatrix2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const mat3& value)   { _Use();   glUniformMatrix3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const mat4& value)   { _Use();   glUniformMatrix4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const mat2x3& value) { _Use(); glUniformMatrix2x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const mat3x2& value) { _Use(); glUniformMatrix3x2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const mat2x4& value) { _Use(); glUniformMatrix2x4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const mat4x2& value) { _Use(); glUniformMatrix4x2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const mat3x4& value) { _Use(); glUniformMatrix3x4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const mat4x3& value) { _Use(); glUniformMatrix4x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }

	inline void operator=(const std::vector<mat2>& value)   { _Use();   glUniformMatrix2fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<mat3>& value)   { _Use();   glUniformMatrix3fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<mat4>& value)   { _Use();   glUniformMatrix4fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<mat2x3>& value) { _Use(); glUniformMatrix2x3fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<mat3x2>& value) { _Use(); glUniformMatrix3x2fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<mat2x4>& value) { _Use(); glUniformMatrix2x4fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<mat4x2>& value) { _Use(); glUniformMatrix4x2fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<mat3x4>& value) { _Use(); glUniformMatrix3x4fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<mat4x3>& value) { _Use(); glUniformMatrix4x3fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }

private:
	GLuint _program;
	GLint _location;

	inline void _Use() { GLShaderProgramBindings::Use(_program); }

};

/**
 * Use this class for a high-level representation of an OpenGL shader. After
 * creating an instance, add shader source by calling AddShaderFromSource(...)
 * or AddShaderFromFile(...). Before the first use, call Link() to actually
 * compile and link the shaders. The function Use() should be used to use this
 * shader program. The operator [] with string arguments can be used to set
 * uniform variables.
 */
class RE_API GLShaderProgram {

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
	 * only add the last-added shader to the program.Note that the file will be
	 * read when the shader is linked, not at the call of this method.
	 */
	void AddShaderFromFile(ShaderType type, std::string file);

	/**
	 * Compiles the added shaders, and links them together.
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
	GLShaderUniform& GetUniform(const char* name) const;

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
