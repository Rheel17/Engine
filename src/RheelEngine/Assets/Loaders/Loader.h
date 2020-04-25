/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_LOADER_H
#define RHEELENGINE_LOADER_H
#include "../../_common.h"

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
	T Load(const std::string& path) {
		auto iter = _cache.find(path);

		if (iter == _cache.end()) {
			Log::Info() << "Loading asset " << path << std::endl;
			iter = _cache.emplace(path, LoaderImpl().Load(path)).first;
		}

		return iter->second;
	}

private:
	std::unordered_map<std::string, T> _cache;

};

}

#endif
