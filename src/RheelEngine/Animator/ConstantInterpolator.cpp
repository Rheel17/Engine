/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "ConstantInterpolator.h"

namespace rheel {

ConstantInterpolator::ConstantInterpolator(std::vector<vec2> points) :
		Interpolator(std::move(points)) {}

}