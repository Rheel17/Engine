/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_
#include "_common.h"

#include <unordered_map>
#include <memory>

#include "ImageResource.h"
#include "ModelResource.h"
#include "SoundResource.h"

namespace rheel {

class RE_API ResourceManager {

public:
	struct ResourceContainer {
		std::unique_ptr<ResourceBase> _resource;
	};

public:
	/**
	 * Sets the path in which the resource manager will look for resource files.
	 * By default, the root folder of the execution environment will be used.
	 */
	static void SetResourcePath(std::string path);

	/**
	 * Registers an image from memory. After this method has been executed, the
	 * image can be loaded using the given path. The resource manager will take
	 * ownership of the pointer.
	 */
	static ImageResource& RegisterImage(const std::string& path, Image *image);

	/**
	 * Registers a model from memory. After this method has been executed, the
	 * model can be loaded using the given path. The resource manager will take
	 * ownership of the pointer.
	 */
	static ModelResource& RegisterModel(const std::string& path, Model *model);

	/**
	 * Loads an image from the file system.
	 */
	static ImageResource& GetImage(const std::string& path);

	/**
	 * Loads a model from the file system.
	 */
	static ModelResource& GetModel(const std::string& path);

	/**
	 * Loads a sound from the file system.
	 */
	static SoundResource& GetSound(const std::string& path);

private:
	template<typename R, typename T>
	static R& _Register(const std::string& path, T *value) {
		auto resource = std::unique_ptr<R>(new R(path, value));
		R& ref = *resource;

		_resource_map.insert({ path, ResourceContainer { std::move(resource) } });
		return ref;
	}

	template<typename T>
	static T& _Get(const std::string& path) {
		auto iter = _resource_map.find(path);

		if (iter != _resource_map.end()) {
			return dynamic_cast<T&>(*iter->second._resource);
		}

		auto resource = std::unique_ptr<T>(new T(path));
		T& ref = *resource;

		_resource_map.insert({ path, ResourceContainer { std::move(resource) } });
		return ref;
	}

	static std::string _resource_path;
	static std::unordered_map<std::string, ResourceContainer> _resource_map;

};

}

#endif
