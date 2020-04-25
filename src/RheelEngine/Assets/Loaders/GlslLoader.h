/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GLSLLOADER_H
#define RHEELENGINE_GLSLLOADER_H
#include "../../_common.h"

#include "Loader.h"
#include "../Shader.h"

namespace rheel {

class RE_API GlslLoader : public AbstractLoader<Shader> {

public:
	Shader Load(const std::string& path) const override;

};

}

#endif
