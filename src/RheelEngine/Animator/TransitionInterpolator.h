/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_TRANSITIONINTERPOLATOR_H
#define RHEELENGINE_TRANSITIONINTERPOLATOR_H
#include "../_common.h"

#include "Interpolator.h"
#include "Transition.h"

namespace rheel {

template<typename V>
class TransitionInterpolator : public Interpolator<V> {

public:
	virtual ~TransitionInterpolator() = default;

	explicit TransitionInterpolator(const Transition& transition) :
			_transition(transition) {}

protected:
	V GetValue(float t) const override {
		auto before = --this->points.upper_bound(t);
		auto after = this->points.upper_bound(t);

		return _transition(t, before->first, after->first, before->second, after->second);
	}

private:
	const Transition& _transition;

};

}

#endif
