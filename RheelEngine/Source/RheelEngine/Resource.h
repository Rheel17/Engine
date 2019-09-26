/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef RESOURCE_H_
#define RESOURCE_H_
#include "_common.h"

#include <memory>

namespace rheel {

class RE_API ResourceBase {

public:
	virtual ~ResourceBase() = default;

};

template<typename T>
class RE_API Resource : public ResourceBase {
	RE_NO_COPY(Resource);
	RE_NO_MOVE(Resource);

public:
	virtual ~Resource() = default;

	const std::string& Name() const {
		return _name;
	}

	T& Get() {
		return *_value;
	}

	const T& Get() const {
		return *_value;
	}

protected:
	Resource(std::string name, T *value) :
		_name(std::move(name)), _value(std::move(value)) {}

private:
	std::string _name;
	std::unique_ptr<T> _value;

};

}

#endif
