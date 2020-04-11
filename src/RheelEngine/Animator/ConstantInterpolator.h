/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CONSTANTINTERPOLATOR_H
#define RHEELENGINE_CONSTANTINTERPOLATOR_H

#include "../_common.h"

#include "Interpolator.h"

namespace rheel {

class RE_API ConstantInterpolator : public Interpolator {

public:
	template<class InputIt>
	ConstantInterpolator(InputIt pointsStart, InputIt pointsEnd) :
		Interpolator(pointsStart, pointsEnd) {}

	explicit ConstantInterpolator(std::vector<vec2> points);

};

}

#endif
