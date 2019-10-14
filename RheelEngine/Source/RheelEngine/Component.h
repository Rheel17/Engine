/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef COMPONENT_H_
#define COMPONENT_H_
#include "_common.h"

#include "Entity.h"

#define COMPONENT_INIT												\
protected:															\
	Component *Clone() const override {								\
		using ThisType__ = std::remove_pointer_t<decltype(this)>;	\
		return new ThisType__(*this);								\
	}																\
private:

namespace rheel {

class RE_API Component {

public:
	virtual ~Component();

protected:


};

}

#endif
