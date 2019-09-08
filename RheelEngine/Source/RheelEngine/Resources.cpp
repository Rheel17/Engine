#include "Resources.h"

namespace rheel {

___res___ Resources::_resources;

const char *Resources::Start(const std::string& resourceName) {
	return _resources.resources_pointers["___binary___" + resourceName + "_start"];
}

size_t Resources::Length(const std::string& resourceName) {
	char *start = _resources.resources_pointers["___binary___" + resourceName + "_start"];
	char *end = _resources.resources_pointers["___binary___" + resourceName + "_end"];

	return end - start;
}

std::string Resources::AsString(const std::string& resourceName) {
	char *start = _resources.resources_pointers["___binary___" + resourceName + "_start"];
	char *end = _resources.resources_pointers["___binary___" + resourceName + "_end"];

	return std::string(start, end);
}

std::string Resources::PreprocessShader(const std::string& resourceName) {
	std::string shader = AsString(resourceName);
	return shader;
}

}
