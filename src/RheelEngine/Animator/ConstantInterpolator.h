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

public:
	T operator()(float t) const override {
		if (this->_points.empty()) {
			return T{};
		}

		if (t <= this->_t_min) {
			return this->_points.begin()->second;
		}

		return (--this->_points.end())->second;
	}

};

}

#endif
