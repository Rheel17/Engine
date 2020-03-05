/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Shader.h"

namespace rheel {

Shader::Shader(std::string source) :
		Asset({ std::move(source) }) {}

const std::string& Shader::GetSource() const {
	return _GetRaw()->source;
}

Shader Shader::Null() {
	return Shader(nullptr);
}

}