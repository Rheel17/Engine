/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "SplineInterpolator.h"

namespace rheel {

SplineInterpolator::SplineInterpolator(std::vector<vec2> points) :
		Interpolator(std::move(points)) {}

}