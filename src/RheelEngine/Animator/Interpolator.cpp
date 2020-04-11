/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Interpolator.h"

namespace rheel {

Interpolator::Interpolator(std::vector<vec2> values) :
		_points(std::move(values)) {}

void Interpolator::_ValidateInput() const {
	float t = std::numeric_limits<float>::lowest();

	for (const auto& point : _points) {
		if (t >= point.x) {
			Log::Error() << "Interpolated points must be specified in order" << std::endl;
			abort();
		}

		t = point.x;
	}
}

}