/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_SPLINEINTERPOLATOR_H
#define RHEELENGINE_SPLINEINTERPOLATOR_H
#include "../_common.h"

#include "Interpolator.h"

namespace rheel {

class RE_API SplineInterpolator : public Interpolator {

public:
	template<class InputIt>
	SplineInterpolator(InputIt pointsStart, InputIt pointsEnd) :
			Interpolator(pointsStart, pointsEnd) {}

	explicit SplineInterpolator(std::vector<vec2> points);

};

}

#endif
