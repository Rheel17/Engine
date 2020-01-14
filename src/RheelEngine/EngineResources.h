/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef ENGINERESOURCES_H_
#define ENGINERESOURCES_H_
#include "_common.h"

#include <_res.h>

namespace rheel {

class RE_API EngineResources {

public:
	static std::string AsString(const std::string& resourceName);

	static std::string PreprocessShader(const std::string& resourceName);

private:
	static std::string _PreprocessShader(const std::string& resourceName, std::unordered_set<std::string>& includedResources);
	static bool _IsShaderIncludeLine(const std::string& line, std::string& resource);

};

}


#endif
