/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_LOADER_H
#define RHEELENGINE_LOADER_H
#include "../../_common.h"

#include <mutex>
#include <condition_variable>

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
		// Actually load the asset. This method might wait for another thread to
		// finish the loading if the asset was preloaded before this point, but
		// hasn't finished loading.
		Preload(path);

		// Acquire the cache lock to ensure no-one is writing to it at the
		// moment and return the asset
		{
			std::lock_guard lock(_mutex);
			return _cache.find(path)->second;
		}
	}

	/**
	 * Loads an asset from the given path. The loader will ensure that the asset
	 * is only loaded from disk once. This method is thread-safe.
	 */
	void Preload(const std::string& path) {
		// First check if the asset is already in the cache or currently being
		// loaded. If it is being loaded, wait for it to be done. If not: add it
		// to the loading set
		{
			std::unique_lock lock(_mutex);

			if (_cache.find(path) != _cache.end()) {
				return;
			}

			if (auto iter = _loading.find(path); iter != _loading.end()) {
				iter->second->wait(lock, [&]() { return _loading.find(path) != _loading.end(); });
				return;
			}

			// insert the path into the loading map, and default-construct the
			// condition_variable.
			_loading[path] = std::make_unique<std::condition_variable>();
			Log::Info() << "Loading asset " << path << std::endl;
		}

		// Load the asset
		T asset = LoaderImpl().Load(path);

		// Add the asset to the cache, remove it from the loading set and notify
		// any waiting threads that the asset has finished loading
		{
			std::lock_guard lock(_mutex);

			// add the asset to the cache
			_cache.insert({ path, std::move(asset) });

			// remove the path from the loading set
			auto conditionVariable = std::move(_loading[path]);
			_loading.erase(path);

			// notify waiting threads
			conditionVariable->notify_all();
		}
	}

private:
	std::unordered_map<std::string, T> _cache;

	std::unordered_map<std::string, std::unique_ptr<std::condition_variable>> _loading;
	std::mutex _mutex;

};

}

#endif
