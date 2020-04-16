/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Uniform.h"

#include "State.h"

namespace rheel::gl {

Uniform::Uniform(GLuint programHandle, const std::string& name, bool checkWarning) :
		_program_handle(programHandle),
		_location(GetUniformLocation(programHandle, name)) {

	if (_location < 0 && checkWarning) {
		Log::Warning() << "shader doesn't have uniform " << name << std::endl;
	}
}

bool Uniform::IsValid() const {
	return _location >= 0;
}

bool Uniform::Init_() const {
	if (IsValid()) {
		State::S_()._bindings.UseProgram(_program_handle);
		return true;
	}

	return false;
}

Uniform& Uniform::operator=(float value) {
	if (Init_()) {
		glUniform1f(_location, value);
	}
	return *this;
}

Uniform& Uniform::operator=(double value) {
	if (Init_()) {
		glUniform1d(_location, value);
	}
	return *this;
}

Uniform& Uniform::operator=(int value) {
	if (Init_()) {
		glUniform1i(_location, value);
	}
	return *this;
}

Uniform& Uniform::operator=(unsigned value) {
	if (Init_()) {
		glUniform1ui(_location, value);
	}
	return *this;
}

Uniform& Uniform::operator=(vec1 value) {
	if (Init_()) {
		glUniform1f(_location, value.x);
	}
	return *this;
}

Uniform& Uniform::operator=(vec2 value) {
	if (Init_()) {
		glUniform2f(_location, value.x, value.y);
	}
	return *this;
}

Uniform& Uniform::operator=(vec3 value) {
	if (Init_()) {
		glUniform3f(_location, value.x, value.y, value.z);
	}
	return *this;
}

Uniform& Uniform::operator=(vec4 value) {
	if (Init_()) {
		glUniform4f(_location, value.x, value.y, value.z, value.w);
	}
	return *this;
}

Uniform& Uniform::operator=(dvec1 value) {
	if (Init_()) {
		glUniform1d(_location, value.x);
	}
	return *this;
}

Uniform& Uniform::operator=(dvec2 value) {
	if (Init_()) {
		glUniform2d(_location, value.x, value.y);
	}
	return *this;
}

Uniform& Uniform::operator=(dvec3 value) {
	if (Init_()) {
		glUniform3d(_location, value.x, value.y, value.z);
	}
	return *this;
}

Uniform& Uniform::operator=(dvec4 value) {
	if (Init_()) {
		glUniform4d(_location, value.x, value.y, value.z, value.w);
	}
	return *this;
}

Uniform& Uniform::operator=(ivec1 value) {
	if (Init_()) {
		glUniform1i(_location, value.x);
	}
	return *this;
}

Uniform& Uniform::operator=(ivec2 value) {
	if (Init_()) {
		glUniform2i(_location, value.x, value.y);
	}
	return *this;
}

Uniform& Uniform::operator=(ivec3 value) {
	if (Init_()) {
		glUniform3i(_location, value.x, value.y, value.z);
	}
	return *this;
}

Uniform& Uniform::operator=(ivec4 value) {
	if (Init_()) {
		glUniform4i(_location, value.x, value.y, value.z, value.w);
	}
	return *this;
}

Uniform& Uniform::operator=(uvec1 value) {
	if (Init_()) {
		glUniform1ui(_location, value.x);
	}
	return *this;
}

Uniform& Uniform::operator=(uvec2 value) {
	if (Init_()) {
		glUniform2ui(_location, value.x, value.y);
	}
	return *this;
}

Uniform& Uniform::operator=(uvec3 value) {
	if (Init_()) {
		glUniform3ui(_location, value.x, value.y, value.z);
	}
	return *this;
}

Uniform& Uniform::operator=(uvec4 value) {
	if (Init_()) {
		glUniform4ui(_location, value.x, value.y, value.z, value.w);
	}
	return *this;
}

Uniform& Uniform::operator=(mat2 value) {
	if (Init_()) {
		glUniformMatrix2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat2x3 value) {
	if (Init_()) {
		glUniformMatrix2x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat2x4 value) {
	if (Init_()) {
		glUniformMatrix2x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat3 value) {
	if (Init_()) {
		glUniformMatrix3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat3x2 value) {
	if (Init_()) {
		glUniformMatrix3x2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat3x4 value) {
	if (Init_()) {
		glUniformMatrix3x4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat4 value) {
	if (Init_()) {
		glUniformMatrix4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat4x2 value) {
	if (Init_()) {
		glUniformMatrix4x2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(mat4x3 value) {
	if (Init_()) {
		glUniformMatrix4x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat2 value) {
	if (Init_()) {
		glUniformMatrix2dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat2x3 value) {
	if (Init_()) {
		glUniformMatrix2x3dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat2x4 value) {
	if (Init_()) {
		glUniformMatrix2x3dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat3 value) {
	if (Init_()) {
		glUniformMatrix3dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat3x2 value) {
	if (Init_()) {
		glUniformMatrix3x2dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat3x4 value) {
	if (Init_()) {
		glUniformMatrix3x4dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat4 value) {
	if (Init_()) {
		glUniformMatrix4dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat4x2 value) {
	if (Init_()) {
		glUniformMatrix4x2dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(dmat4x3 value) {
	if (Init_()) {
		glUniformMatrix4x3dv(_location, 1, GL_FALSE, reinterpret_cast<const GLdouble*>(&value[0][0]));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<float>& values) {
	if (Init_()) {
		glUniform1fv(_location, values.size(), values.data());
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<double>& values) {
	if (Init_()) {
		glUniform1dv(_location, values.size(), values.data());
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<int>& values) {
	if (Init_()) {
		glUniform1iv(_location, values.size(), values.data());
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<unsigned>& values) {
	if (Init_()) {
		glUniform1uiv(_location, values.size(), values.data());
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<vec1>& values) {
	if (Init_()) {
		glUniform1fv(_location, values.size(), reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<vec2>& values) {
	if (Init_()) {
		glUniform2fv(_location, values.size(), reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<vec3>& values) {
	if (Init_()) {
		glUniform3fv(_location, values.size(), reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<vec4>& values) {
	if (Init_()) {
		glUniform4fv(_location, values.size(), reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dvec1>& values) {
	if (Init_()) {
		glUniform1dv(_location, values.size(), reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dvec2>& values) {
	if (Init_()) {
		glUniform2dv(_location, values.size(), reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dvec3>& values) {
	if (Init_()) {
		glUniform3dv(_location, values.size(), reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dvec4>& values) {
	if (Init_()) {
		glUniform4dv(_location, values.size(), reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<ivec1>& values) {
	if (Init_()) {
		glUniform1iv(_location, values.size(), reinterpret_cast<const GLint*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<ivec2>& values) {
	if (Init_()) {
		glUniform2iv(_location, values.size(), reinterpret_cast<const GLint*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<ivec3>& values) {
	if (Init_()) {
		glUniform3iv(_location, values.size(), reinterpret_cast<const GLint*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<ivec4>& values) {
	if (Init_()) {
		glUniform4iv(_location, values.size(), reinterpret_cast<const GLint*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<uvec1>& values) {
	if (Init_()) {
		glUniform1uiv(_location, values.size(), reinterpret_cast<const GLuint*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<uvec2>& values) {
	if (Init_()) {
		glUniform2uiv(_location, values.size(), reinterpret_cast<const GLuint*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<uvec3>& values) {
	if (Init_()) {
		glUniform3uiv(_location, values.size(), reinterpret_cast<const GLuint*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<uvec4>& values) {
	if (Init_()) {
		glUniform4uiv(_location, values.size(), reinterpret_cast<const GLuint*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat2>& values) {
	if (Init_()) {
		glUniformMatrix2fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat2x3>& values) {
	if (Init_()) {
		glUniformMatrix2x3fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat2x4>& values) {
	if (Init_()) {
		glUniformMatrix2x3fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat3>& values) {
	if (Init_()) {
		glUniformMatrix3fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat3x2>& values) {
	if (Init_()) {
		glUniformMatrix3x2fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat3x4>& values) {
	if (Init_()) {
		glUniformMatrix3x4fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat4>& values) {
	if (Init_()) {
		glUniformMatrix4fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat4x2>& values) {
	if (Init_()) {
		glUniformMatrix4x2fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<mat4x3>& values) {
	if (Init_()) {
		glUniformMatrix4x3fv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLfloat*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat2>& values) {
	if (Init_()) {
		glUniformMatrix2dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat2x3>& values) {
	if (Init_()) {
		glUniformMatrix2x3dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat2x4>& values) {
	if (Init_()) {
		glUniformMatrix2x3dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat3>& values) {
	if (Init_()) {
		glUniformMatrix3dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat3x2>& values) {
	if (Init_()) {
		glUniformMatrix3x2dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat3x4>& values) {
	if (Init_()) {
		glUniformMatrix3x4dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat4>& values) {
	if (Init_()) {
		glUniformMatrix4dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat4x2>& values) {
	if (Init_()) {
		glUniformMatrix4x2dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

Uniform& Uniform::operator=(const std::vector<dmat4x3>& values) {
	if (Init_()) {
		glUniformMatrix4x3dv(_location, values.size(), GL_FALSE, reinterpret_cast<const GLdouble*>(values.data()));
	}
	return *this;
}

GLint Uniform::GetUniformLocation(GLuint programHandle, const std::string& name) {
	return glGetUniformLocation(programHandle, name.c_str());
}

}
