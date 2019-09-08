#ifndef RESOURCES_H_
#define RESOURCES_H_
#include "_common.h"

#include <resources.h>

#include <string>
#include <set>

namespace rheel {

class RE_API Resources {

public:
	static const char *Start(const std::string& resourceName);
	static size_t Length(const std::string& resourceName);
	static std::string AsString(const std::string& resourceName);
	static std::string PreprocessShader(const std::string& resourceName);

private:
	static std::string _PreprocessShader(const std::string& resourceName, std::set<std::string>& includedResources);
	static bool _IsShaderIncludeLine(const std::string& line, std::string& resource);

	static ___res___ _resources;

};

}


#endif
