/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "EngineResources.h"

#include <sstream>

namespace rheel {

std::string EngineResources::AsString(const std::string& resourceName) {
	return ___res___::Get(resourceName);
}

std::string EngineResources::PreprocessShader(const std::string& resourceName) {
	std::unordered_set<std::string> includedShaders;
	return _PreprocessShader(resourceName, includedShaders);
}

std::string EngineResources::_PreprocessShader(const std::string& resourceName, std::unordered_set<std::string>& includedResources) {
	includedResources.insert(resourceName);

	std::stringstream reader(AsString(resourceName));
	std::stringstream builder;
	std::string line;
	std::string includeResource;

	while (std::getline(reader, line, '\n')) {
		if (_IsShaderIncludeLine(line, includeResource)) {
			if (includedResources.find(includeResource) == includedResources.end()) {
				builder << _PreprocessShader(includeResource, includedResources);
			}
		} else {
			builder << line << '\n';
		}
	}

	return builder.str();
}

bool EngineResources::_IsShaderIncludeLine(const std::string& line, std::string& resource) {
	std::stringstream lineInput(line);
	std::string word;
	int state = 0;

	while (state <= 3) {
		word.clear();
		lineInput >> word;

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