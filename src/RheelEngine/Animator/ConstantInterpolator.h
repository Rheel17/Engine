/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CONSTANTINTERPOLATOR_H
#define RHEELENGINE_CONSTANTINTERPOLATOR_H

#include "../_common.h"

#include "Interpolator.h"

namespace rheel {

template<typename T>
class RE_API ConstantInterpolator : public Interpolator<T> {

protected:
	T _GetValue(float t) const override {
		return (--this->_points.end())->second;
	}

};

}

#endif
