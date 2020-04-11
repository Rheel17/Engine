/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_INTERPOLATOR_H
#define RHEELENGINE_INTERPOLATOR_H

#include "../_common.h"

namespace rheel {

class RE_API Interpolator {

public:
	~Interpolator() = default;

	virtual float operator()(float t) const = 0;

protected:
	template<class InputIt>
	Interpolator(InputIt pointsStart, InputIt pointsEnd) :
			_points(pointsStart, pointsEnd) {}

	explicit Interpolator(std::vector<vec2> points);

private:
	std::vector<vec2> _points;

	void _ValidateInput() const;

};

}

#endif
