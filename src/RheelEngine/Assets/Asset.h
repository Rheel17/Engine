/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_ASSET_H
#define RHEELENGINE_ASSET_H
#include "../_common.h"

namespace rheel {

class RE_API AssetBase {};

template<typename T>
class RE_API Asset : public AssetBase {

public:
	virtual ~Asset() = default;

	/*
	 * Checks whether this asset is empty (null)
	 */
	bool IsNull() const {
		return _data == nullptr;
	}

	/*
	 * Requires that this asset is not empty (null). Will throw an exception if this
	 * asset is empty (null).
	 */
	void RequireNonNull() const {
		if (IsNull()) {
			throw std::runtime_error("Asset is null");
		}
	}

	std::uintptr_t GetAddress() const {
		return std::uintptr_t(_data.get());
	}

	/*
	 * The equality operator uses raw addresses to check for equality. This means that
	 * two assets that are semantically the same (have the same data) might not
	 * return true for this function.
	 *
	 * To check for null assets, use this IsNull() or RequireNonNull() functions.
	 */
	template<typename T2>
	bool operator==(const Asset& rhs) const {
		return GetAddress() == rhs.GetAddress();
	}

	/*
	 * The equality operator uses raw addresses to check for equality. This means that
	 * two assets that are semantically the same (have the same data) might not
	 * return true for this function.
	 *
	 * To check for null assets, use this IsNull() or RequireNonNull() functions.
	 */
	template<typename T2>
	bool operator!=(const Asset<T2>& rhs) const {
		return !(rhs == *this);
	}

protected:
	Asset() : _data(std::make_shared<T>()) {}

	explicit Asset(T&& t) :
			_data(std::make_shared<T>(std::forward<T>(t))) {}

	explicit Asset(std::nullptr_t null) :
			_data(nullptr) {}

	/*
	 * Use with caution! The data could be null.
	 */
	const T* _GetRaw() const {
		return _data.get();
	}

	/*
	 * Use with caution! The data could be null.
	 */
	T* _GetRaw() {
		return _data.get();
	}

private:
	std::shared_ptr<T> _data;

};

}

#endif
