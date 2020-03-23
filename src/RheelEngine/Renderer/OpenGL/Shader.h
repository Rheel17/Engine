/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_SHADER_H
#define RHEELENGINE_GL_SHADER_H
#include "../../_common.h"

#include <gl/glew.h>

namespace rheel::GL {

class RE_API Shader {
	RE_NO_COPY(Shader);

public:
	enum class ShaderType {
		COMPUTE = GL_COMPUTE_SHADER, VERTEX = GL_VERTEX_SHADER, TESS_CONTROL = GL_TESS_CONTROL_SHADER,
		TESS_EVALUATION = GL_TESS_EVALUATION_SHADER, GEOMETRY = GL_GEOMETRY_SHADER, FRAGMENT = GL_FRAGMENT_SHADER
	};

public:
	explicit Shader(ShaderType type, const std::string& source);
	~Shader();

	Shader(Shader&& s) noexcept;
	Shader& operator=(Shader&& s) noexcept;

	GLuint GetHandle() const;

	ShaderType GetType() const;

private:
	GLuint _handle;
	ShaderType _type;

};

}

#endif
