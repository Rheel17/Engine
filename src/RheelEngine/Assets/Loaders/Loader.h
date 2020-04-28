/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_LOADER_H
#define RHEELENGINE_LOADER_H
#include "../../_common.h"

#include "../../Util/Cache.h"

namespace rheel {

template<typename T>
class AbstractLoader {

public:
	using Type = T;

	virtual ~AbstractLoader() = default;
	virtual T Load(const std::string& path) const = 0;

};

template<typename T, typename LoaderImpl, std::enable_if_t<std::is_base_of_v<AbstractLoader<T>, LoaderImpl>, int> = 0>
class RE_API Loader {
	RE_NO_MOVE(Loader);
	RE_NO_COPY(Loader);

private:
	friend class AssetLoader;

	Loader() = default;

public:
	/**
	 * Loads an asset from the given path. The loader will ensure that the asset
	 * is only loaded from disk once. This method is thread-safe.
	 */
	T Load(const std::string& path) {
		return _cache.Get(path, Load_);
	}

	/**
	 * Loads an asset from the given path. The loader will ensure that the asset
	 * is only loaded from disk once. This method is thread-safe.
	 */
	void Preload(const std::string& path) {
		_cache.Put(path, Load_);
	}

private:
	Cache<std::string, T, keep_policy> _cache;

private:
	static T Load_(const std::string& path) {
		Log::Info() << "Loading asset " << path << std::endl;
		return LoaderImpl().Load(path);
	}

};

}

#endif
