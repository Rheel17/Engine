/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef COMPONENT_H_
#define COMPONENT_H_
#include "_common.h"

#include "Entity.h"
#include "Transform.h"

namespace rheel {

/**
 * Base class of all components. Do not use this class as a direct base class;
 * use Component or RigidComponent instead.
 */
class RE_API ComponentBase {
	template<typename _Transform>
	friend class _Component;

public:
	virtual ~ComponentBase() = default;

private:
	ComponentBase() = default;

};

/**
 * General base class for components. Most commonly, you want to use Component
 * or RigidComponent as actual base classes for custom components.
 */
template<typename _Transform>
class RE_API _Component : public ComponentBase {
	static_assert(std::is_base_of_v<RigidTransform, _Transform>, "Component transform must extend RigidTransform");

public:
	virtual ~_Component() = default;

};

using Component = _Component<Transform>;
using RigidComponent = _Component<RigidTransform>;

}

#endif
