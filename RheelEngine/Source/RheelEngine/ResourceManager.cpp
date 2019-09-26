/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "ResourceManager.h"

namespace rheel {

std::string ResourceManager::_resource_path;
std::unordered_map<std::string, ResourceManager::ResourceContainer> ResourceManager::_resource_map;

void ResourceManager::SetResourcePath(std::string path) {
	_resource_path = std::move(path);
}

ImageResource& ResourceManager::RegisterImage(const std::string& path, Image *image) {
	return _Register<ImageResource, Image>(path, image);
}

ModelResource& ResourceManager::RegisterModel(const std::string& path, Model *model) {
	return _Register<ModelResource, Model>(path, model);
}

ImageResource& ResourceManager::GetImage(const std::string& path) {
	return _Get<ImageResource>(_resource_path + "/" + path);
}

ModelResource& ResourceManager::GetModel(const std::string& path) {
	return _Get<ModelResource>(_resource_path + "/" + path);
}

}
