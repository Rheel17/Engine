/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_COSINEINTERPOLATOR_H
#define RHEELENGINE_COSINEINTERPOLATOR_H
#include "../_common.h"

#include "Interpolator.h"

namespace rheel {

class RE_API CosineInterpolator : public Interpolator {

public:
	template<class InputIt>
	CosineInterpolator(InputIt pointsStart, InputIt pointsEnd) :
			Interpolator(pointsStart, pointsEnd) {}

	explicit CosineInterpolator(std::vector<vec2> points);

};

}

#endif
