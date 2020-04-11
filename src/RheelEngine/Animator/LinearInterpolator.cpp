/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "LinearInterpolator.h"

namespace rheel {

LinearInterpolator::LinearInterpolator(std::vector<vec2> points) :
		Interpolator(std::move(points)) {}

}