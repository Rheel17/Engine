#include "ModelResource.h"

namespace rheel {

ModelResource::ModelResource(const std::string& path) :
		Resource(path, new Model(path, Model::FormatCollada)) {

	// TODO: automatically find correct model format based on file extension
}

ModelResource::ModelResource(const std::string& path, Model *model) :
		Resource(path, model) {}

ModelResource::~ModelResource() {}

}
