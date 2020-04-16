/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_ENGINERESOURCES_H
#define RHEELENGINE_ENGINERESOURCES_H
#include "_common.h"

namespace rheel {

class RE_API EngineResources {

public:
	static std::string AsString(const std::string& resourceName);

	static std::string PreprocessShader(const std::string& resourceName);

private:
	static std::string PreprocessShader_(const std::string& resourceName, std::unordered_set<std::string>& includedResources);
	static bool IsShaderIncludeLine_(const std::string& line, std::string& resource);

};

}

#endif
