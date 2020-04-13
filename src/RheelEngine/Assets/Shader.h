/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_SHADER_H
#define RHEELENGINE_SHADER_H
#include "../_common.h"

#include "Asset.h"

namespace rheel {

struct _ShaderData {
	std::string source;
};

class RE_API Shader : public Asset<_ShaderData> {

public:
	explicit Shader(std::string source);

	const std::string& GetSource() const;

private:
	Shader() = default;
	Shader(std::nullptr_t null) : Asset(nullptr) {}

public:
	static Shader Null();

};

}

#endif