/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_LOADER_H
#define RHEELENGINE_LOADER_H
#include "../../_common.h"

namespace rheel {

template<typename T>
class RE_API Loader {

public:
	virtual ~Loader() = default;

	T Load(const std::string& path) {
		auto iter = _cache.find(path);

		if (iter == _cache.end()) {
			iter = _cache.emplace(path, _DoLoad(path)).first;
		}

		return iter->second;
	}

protected:
	Loader() = default;

	virtual T _DoLoad(const std::string& path) = 0;

private:
	std::unordered_map<std::string, T> _cache;

};

}

#endif
