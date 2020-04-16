/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_UNIFORM_H
#define RHEELENGINE_UNIFORM_H
#include "../../_common.h"

#include <gl/glew.h>

namespace rheel::gl {

class RE_API Uniform {
	friend class Program;

	RE_NO_COPY(Uniform);
	RE_NO_MOVE(Uniform);

public:
	Uniform(GLuint programHandle, const std::string& name, bool checkWarning);

	bool IsValid() const;

	Uniform& operator=(float value);
	Uniform& operator=(double value);
	Uniform& operator=(int value);
	Uniform& operator=(unsigned value);

	Uniform& operator=(vec1 value);
	Uniform& operator=(vec2 value);
	Uniform& operator=(vec3 value);
	Uniform& operator=(vec4 value);

	Uniform& operator=(dvec1 value);
	Uniform& operator=(dvec2 value);
	Uniform& operator=(dvec3 value);
	Uniform& operator=(dvec4 value);

	Uniform& operator=(ivec1 value);
	Uniform& operator=(ivec2 value);
	Uniform& operator=(ivec3 value);
	Uniform& operator=(ivec4 value);

	Uniform& operator=(uvec1 value);
	Uniform& operator=(uvec2 value);
	Uniform& operator=(uvec3 value);
	Uniform& operator=(uvec4 value);

	Uniform& operator=(mat2 value);
	Uniform& operator=(mat2x3 value);
	Uniform& operator=(mat2x4 value);
	Uniform& operator=(mat3 value);
	Uniform& operator=(mat3x2 value);
	Uniform& operator=(mat3x4 value);
	Uniform& operator=(mat4 value);
	Uniform& operator=(mat4x2 value);
	Uniform& operator=(mat4x3 value);

	Uniform& operator=(dmat2 value);
	Uniform& operator=(dmat2x3 value);
	Uniform& operator=(dmat2x4 value);
	Uniform& operator=(dmat3 value);
	Uniform& operator=(dmat3x2 value);
	Uniform& operator=(dmat3x4 value);
	Uniform& operator=(dmat4 value);
	Uniform& operator=(dmat4x2 value);
	Uniform& operator=(dmat4x3 value);

	Uniform& operator=(const std::vector<float>& values);
	Uniform& operator=(const std::vector<double>& values);
	Uniform& operator=(const std::vector<int>& values);
	Uniform& operator=(const std::vector<unsigned>& values);

	Uniform& operator=(const std::vector<vec1>& values);
	Uniform& operator=(const std::vector<vec2>& values);
	Uniform& operator=(const std::vector<vec3>& values);
	Uniform& operator=(const std::vector<vec4>& values);

	Uniform& operator=(const std::vector<dvec1>& values);
	Uniform& operator=(const std::vector<dvec2>& values);
	Uniform& operator=(const std::vector<dvec3>& values);
	Uniform& operator=(const std::vector<dvec4>& values);

	Uniform& operator=(const std::vector<ivec1>& values);
	Uniform& operator=(const std::vector<ivec2>& values);
	Uniform& operator=(const std::vector<ivec3>& values);
	Uniform& operator=(const std::vector<ivec4>& values);

	Uniform& operator=(const std::vector<uvec1>& values);
	Uniform& operator=(const std::vector<uvec2>& values);
	Uniform& operator=(const std::vector<uvec3>& values);
	Uniform& operator=(const std::vector<uvec4>& values);

	Uniform& operator=(const std::vector<mat2>& value);
	Uniform& operator=(const std::vector<mat2x3>& value);
	Uniform& operator=(const std::vector<mat2x4>& value);
	Uniform& operator=(const std::vector<mat3>& value);
	Uniform& operator=(const std::vector<mat3x2>& value);
	Uniform& operator=(const std::vector<mat3x4>& value);
	Uniform& operator=(const std::vector<mat4>& value);
	Uniform& operator=(const std::vector<mat4x2>& value);
	Uniform& operator=(const std::vector<mat4x3>& value);

	Uniform& operator=(const std::vector<dmat2>& value);
	Uniform& operator=(const std::vector<dmat2x3>& value);
	Uniform& operator=(const std::vector<dmat2x4>& value);
	Uniform& operator=(const std::vector<dmat3>& value);
	Uniform& operator=(const std::vector<dmat3x2>& value);
	Uniform& operator=(const std::vector<dmat3x4>& value);
	Uniform& operator=(const std::vector<dmat4>& value);
	Uniform& operator=(const std::vector<dmat4x2>& value);
	Uniform& operator=(const std::vector<dmat4x3>& value);

private:
	bool Init_() const;

	GLuint _program_handle;
	GLint _location;

public:
	static GLint GetUniformLocation(GLuint programHandle, const std::string& name);

};

}

#endif
