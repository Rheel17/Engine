/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_TRANSITION_H
#define RHEELENGINE_TRANSITION_H
#include "../_common.h"

namespace rheel {

class RE_API Transition {
	RE_NO_COPY(Transition)
	RE_NO_MOVE(Transition)

private:
	typedef float (*_TransitionFunction)(float t);

	explicit Transition(_TransitionFunction function);

	_TransitionFunction _function;

public:
	template<typename V>
	V operator()(float t, float tMin, float tMax, V vMin, V vMax) const {
		return _function((t - tMin) / (tMax - tMin)) * (vMax - vMin) + vMin;
	}

public:
	static const Transition linear;
	static const Transition quadratic_in;
	static const Transition quadratic_out;
	static const Transition cubic;
	static const Transition cosine;

};

}

#endif
