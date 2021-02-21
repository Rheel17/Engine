/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_ENGINERESOURCES_H
#define RHEELENGINE_ENGINERESOURCES_H
#include "_common.h"

namespace rheel {

class RE_API EngineResources {

public:
	static std::string AsString(const std::string& resource_name);

	static std::string PreprocessShader(const std::string& resource_name);

private:
	static std::string _preprocess_shader(const std::string& resource_name, std::unordered_set<std::string>& included_resources);
	static bool _is_shader_include_line(const std::string& line, std::string& resource);

};

}

#endif
