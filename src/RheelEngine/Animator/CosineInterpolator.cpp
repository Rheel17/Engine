/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "CosineInterpolator.h"

namespace rheel {

CosineInterpolator::CosineInterpolator(std::vector<vec2> points) :
		Interpolator(std::move(points)) {}

}