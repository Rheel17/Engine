#ifndef MODELRESOURCE_H_
#define MODELRESOURCE_H_
#include "_common.h"

#include "Model.h"
#include "Resource.h"

namespace rheel {

class RE_API ModelResource : public Resource<Model> {
	RE_NO_COPY(ModelResource);
	RE_NO_MOVE(ModelResource);

	friend class ResourceManager;

public:
	~ModelResource();

private:
	ModelResource(const std::string& path);
	ModelResource(const std::string& path, Model *model);

};

}

#endif
