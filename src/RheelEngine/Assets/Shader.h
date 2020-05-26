/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_SHADER_H
#define RHEELENGINE_SHADER_H
#include "../_common.h"

#include "Asset.h"

namespace rheel {

struct shader_data {
	std::string source;
};

class RE_API Shader : public Asset<shader_data> {

public:
	explicit Shader(std::string source);

	const std::string& GetSource() const;

private:
	Shader() = default;
	Shader(std::nullptr_t) :
			Asset(nullptr) {}

public:
	static Shader Null();

};

}

#endif
