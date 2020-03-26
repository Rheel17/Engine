/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Uniform.h"

#include "State.h"

namespace rheel::GL {

Uniform::Uniform(GLuint programHandle, const std::string& name) :
		_program_handle(programHandle), _location(GetUniformLocation(programHandle, name)) {

	if (_location < 0) {
		Log::Warning() << "shader doesn't have uniform " << name << std::endl;
	}
}

bool Uniform::_Init() const {
	if (_location >= 0) {
		State::_S()._bindings.UseProgram(_program_handle);
		return true;
	}

	return false;
}

Uniform& Uniform::operator=(float value) {
	if (_Init()) {
		glUniform1f(_location, value);
	}
	return *this;
}

Uniform& Uniform::operator=(double value) {
	if (_Init()) {
		glUniform1d(_location, value);
	}
	return *this;
}

Uniform& Uniform::operator=(int value) {
	if (_Init()) {
		glUniform1i(_location, value);
	}
	return *this;
}

Uniform& Uniform::operator=(unsigned value) {
	if (_Init()) {
		glUniform1ui(_location, value);
	}
	return *this;
}

Uniform& Uniform::operator=(vec1 value) {
	if (_Init()) {
		glUniform1f(_location, value.x);
	}
	return *this;
}

Uniform& Uniform::operator=(vec2 value) {
	if (_Init()) {
		glUniform2f(_location, value.x, value.y);
	}
	return *this;
}

Uniform& Uniform::operator=(vec3 value) {
	if (_Init()) {
		glUniform3f(_location, value.x, value.y, value.z);
	}
	return *this;
}

Uniform& Uniform::operator=(vec4 value) {
	if (_Init()) {
		glUniform4f(_location, value.x, value.y, value.z, value.w);
	}
	return *this;
}

Uniform& Uniform::operator=(dvec1 value) {
	if (_Init()) {
		glUniform1d(_location, value.x);
	}
	return *this;
}

Uniform& Uniform::operator=(dvec2 value) {
	if (_Init()) {
		glUniform2d(_location, value.x, value.y);
	}
	return *this;
}

Uniform& Uniform::operator=(dvec3 value) {
	if (_Init()) {
		glUniform3d(_location, value.x, value.y, value.z);
	}
	return *this;
}

Uniform& Uniform::operator=(dvec4 value) {
	if (_Init()) {
		glUniform4d(_location, value.x, value.y, value.z, value.w);
	}
	return *this;
}

Uniform& Uniform::operator=(ivec1 value) {
	if (_Init()) {
		glUniform1i(_location, value.x);
	}
	return *this;
}

Uniform& Uniform::operator=(ivec2 value) {
	if (_Init()) {
		glUniform2i(_location, value.x, value.y);
	}
	return *this;
}

Uniform& Uniform::operator=(ivec3 value) {
	if (_Init()) {
		glUniform3i(_location, value.x, value.y, value.z);
	}
	return *this;
}

Uniform& Uniform::operator=(ivec4 value) {
	if (_Init()) {
		glUniform4i(_location, value.x, value.y, value.z, value.w);
	}
	return *this;
}

Uniform& Uniform::operator=(uvec1 value) {
	if (_Init()) {
		glUniform1ui(_location, value.x);
	}
	return *this;
}

Uniform& Uniform::operator=(uvec2 value) {
	if (_Init()) {
		glUniform2ui(_location, value.x, value.y);
	}
	return *this;
}

Uniform& Uniform::operator=(uvec3 value) {
	if (_Init()) {
		glUniform3ui(_location, value.x, value.y, value.z);
	}
	return *this;
}

Uniform& Uniform::operator=(uvec4 value) {
	if (_Init()) {
		glUniform4ui(_location, value.x, value.y, value.z, value.w);
	}
	return *this;
}

Uniform& Uniform::operator=(mat2 value) {
	if (_Init()) {
		glUniformMatrix2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat2x3 value) {
	if (_Init()) {
		glUniformMatrix2x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat2x4 value) {
	if (_Init()) {
		glUniformMatrix2x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat3 value) {
	if (_Init()) {
		glUniformMatrix3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat3x2 value) {
	if (_Init()) {
		glUniformMatrix3x2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat3x4 value) {
	if (_Init()) {
		glUniformMatrix3x4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat4 value) {
	if (_Init()) {
		glUniformMatrix4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat4x2 value) {
	if (_Init()) {
		glUniformMatrix4x2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat4x3 value) {
	if (_Init()) {
		glUniformMatrix4x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat2 value) {
	if (_Init()) {
		glUniformMatrix2dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat2x3 value) {
	if (_Init()) {
		glUniformMatrix2x3dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat2x4 value) {
	if (_Init()) {
		glUniformMatrix2x3dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat3 value) {
	if (_Init()) {
		glUniformMatrix3dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat3x2 value) {
	if (_Init()) {
		glUniformMatrix3x2dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat3x4 value) {
	if (_Init()) {
		glUniformMatrix3x4dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat4 value) {
	if (_Init()) {
		glUniformMatrix4dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat4x2 value) {
	if (_Init()) {
		glUniformMatrix4x2dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat4x3 value) {
	if (_Init()) {
		glUniformMatrix4x3dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble *>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<float>& values) {
	if (_Init()) {
		glUniform1fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<double>& values) {
	if (_Init()) {
		glUniform1dv(_location, values.size(), reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<int>& values) {
	if (_Init()) {
		glUniform1iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<unsigned>& values) {
	if (_Init()) {
		glUniform1uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<vec1>& values) {
	if (_Init()) {
		glUniform1fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<vec2>& values) {
	if (_Init()) {
		glUniform2fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<vec3>& values) {
	if (_Init()) {
		glUniform3fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<vec4>& values) {
	if (_Init()) {
		glUniform4fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dvec1>& values) {
	if (_Init()) {
		glUniform1dv(_location, values.size(), reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dvec2>& values) {
	if (_Init()) {
		glUniform2dv(_location, values.size(), reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dvec3>& values) {
	if (_Init()) {
		glUniform3dv(_location, values.size(), reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dvec4>& values) {
	if (_Init()) {
		glUniform4dv(_location, values.size(), reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<ivec1>& values) {
	if (_Init()) {
		glUniform1iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<ivec2>& values) {
	if (_Init()) {
		glUniform2iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<ivec3>& values) {
	if (_Init()) {
		glUniform3iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<ivec4>& values) {
	if (_Init()) {
		glUniform4iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<uvec1>& values) {
	if (_Init()) {
		glUniform1uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<uvec2>& values) {
	if (_Init()) {
		glUniform2uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<uvec3>& values) {
	if (_Init()) {
		glUniform3uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<uvec4>& values) {
	if (_Init()) {
		glUniform4uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat2>& values) {
	if (_Init()) {
		glUniformMatrix2fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat2x3>& values) {
	if (_Init()) {
		glUniformMatrix2x3fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat2x4>& values) {
	if (_Init()) {
		glUniformMatrix2x3fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat3>& values) {
	if (_Init()) {
		glUniformMatrix3fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat3x2>& values) {
	if (_Init()) {
		glUniformMatrix3x2fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat3x4>& values) {
	if (_Init()) {
		glUniformMatrix3x4fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat4>& values) {
	if (_Init()) {
		glUniformMatrix4fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat4x2>& values) {
	if (_Init()) {
		glUniformMatrix4x2fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat4x3>& values) {
	if (_Init()) {
		glUniformMatrix4x3fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat2>& values) {
	if (_Init()) {
		glUniformMatrix2dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat2x3>& values) {
	if (_Init()) {
		glUniformMatrix2x3dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat2x4>& values) {
	if (_Init()) {
		glUniformMatrix2x3dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat3>& values) {
	if (_Init()) {
		glUniformMatrix3dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat3x2>& values) {
	if (_Init()) {
		glUniformMatrix3x2dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat3x4>& values) {
	if (_Init()) {
		glUniformMatrix3x4dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat4>& values) {
	if (_Init()) {
		glUniformMatrix4dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat4x2>& values) {
	if (_Init()) {
		glUniformMatrix4x2dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat4x3>& values) {
	if (_Init()) {
		glUniformMatrix4x3dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble *>(values.data()));
	}
	return *this;
}

GLint Uniform::GetUniformLocation(GLuint programHandle, const std::string& name) {
	return glGetUniformLocation(programHandle, name.c_str());
}

}
