/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_LINEARINTERPOLATOR_H
#define RHEELENGINE_LINEARINTERPOLATOR_H
#include "../_common.h"

#include "Interpolator.h"

namespace rheel {

class RE_API LinearInterpolator : public Interpolator {

public:
	template<class InputIt>
	LinearInterpolator(InputIt pointsStart, InputIt pointsEnd) :
			Interpolator(pointsStart, pointsEnd) {}

	explicit LinearInterpolator(std::vector<vec2> points);

};

}

#endif
