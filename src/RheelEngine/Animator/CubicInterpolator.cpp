/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "CubicInterpolator.h"

namespace rheel {

CubicInterpolator::CubicInterpolator(std::vector<vec2> points) :
		Interpolator(std::move(points)) {}

}