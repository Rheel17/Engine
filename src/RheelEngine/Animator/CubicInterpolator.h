/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CUBICINTERPOLATOR_H
#define RHEELENGINE_CUBICINTERPOLATOR_H
#include "../_common.h"

#include "Interpolator.h"

namespace rheel {

class RE_API CubicInterpolator : public Interpolator {

public:
	template<class InputIt>
	CubicInterpolator(InputIt pointsStart, InputIt pointsEnd) :
			Interpolator(pointsStart, pointsEnd) {}

	explicit CubicInterpolator(std::vector<vec2> points);

};

}

#endif
