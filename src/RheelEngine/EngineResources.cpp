/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "EngineResources.h"

#include <sstream>

#include <_res.h>

namespace rheel {

std::string EngineResources::AsString(const std::string& resource_name) {
	return ___res___::Get(resource_name);
}

std::string EngineResources::PreprocessShader(const std::string& resource_name) {
	std::unordered_set<std::string> included_shaders;
	return _preprocess_shader(resource_name, included_shaders);
}

std::string EngineResources::_preprocess_shader(const std::string& resource_name, std::unordered_set<std::string>& included_resources) {
	included_resources.insert(resource_name);

	std::stringstream reader(AsString(resource_name));
	std::stringstream builder;
	std::string line;
	std::string include_resource;

	while (std::getline(reader, line, '\n')) {
		if (_is_shader_include_line(line, include_resource)) {
			if (included_resources.find(include_resource) == included_resources.end()) {
				builder << _preprocess_shader(include_resource, included_resources);
			}
		} else {
			builder << line << '\n';
		}
	}

	return builder.str();
}

bool EngineResources::_is_shader_include_line(const std::string& line, std::string& resource) {
	std::stringstream line_input(line);
	std::string word;
	int state = 0;

	while (state <= 3) {
		word.clear();
		line_input >> word;

		if (word.empty()) {
			return false;
		}

		if (state == 0) {
			if (word == "#") {
				state = 1;
				continue;
			} else if (word == "#pragma") {
				state = 2;
				continue;
			}
		} else if (state == 1) {
			if (word == "pragma") {
				state = 2;
				continue;
			}
		} else if (state == 2) {
			if (word == "include") {
				state = 3;
				continue;
			}
		} else {
			resource = word;
			return true;
		}

		return false;
	}

	return false;
}

}
