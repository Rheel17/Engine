#ifndef RESOURCES_H_
#define RESOURCES_H_
#include "_common.h"

#include <resources.h>

#include <string>

#define RESOURCE_AS_STRING(resource) \
	std::string(RESOURCE_START(resource), RESOURCE_END(resource))


#endif
