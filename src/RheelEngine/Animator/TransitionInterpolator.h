/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_TRANSITIONINTERPOLATOR_H
#define RHEELENGINE_TRANSITIONINTERPOLATOR_H
#include "../_common.h"

#include "Interpolator.h"
#include "Transition.h"

namespace rheel {

template<typename T>
class RE_API TransitionInterpolator : public Interpolator<T> {

public:
	virtual ~TransitionInterpolator() = default;

protected:
	explicit TransitionInterpolator(const Transition& transition) :
			_transition(transition) {}

	T _GetValue(float t) const override {
		auto before = --this->_points.upper_bound(t);
		auto after = this->_points.upper_bound(t);

		return _transition(t, before->first, after->first, before->second, after->second);
	}

private:
	const Transition& _transition;

};

}

#endif
