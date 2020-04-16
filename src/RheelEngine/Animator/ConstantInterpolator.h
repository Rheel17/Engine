/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CONSTANTINTERPOLATOR_H
#define RHEELENGINE_CONSTANTINTERPOLATOR_H

#include "../_common.h"

#include "Interpolator.h"

namespace rheel {

template<typename V>
class RE_API ConstantInterpolator : public Interpolator<V> {

protected:
	V _GetValue(float t) const override {
		return (--this->points.end())->second;
	}

};

}

#endif
