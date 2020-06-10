/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_TRANSITION_H
#define RHEELENGINE_TRANSITION_H
#include "../_common.h"

namespace rheel {

class RE_API Transition {
	RE_NO_COPY(Transition);
	RE_NO_MOVE(Transition);

private:
	typedef float (* TransitionFunction)(float t);

	explicit Transition(TransitionFunction function);

	TransitionFunction _function;

public:
	template<typename V>
	V operator()(float t, float tMin, float tMax, V vMin, V vMax) const {
		return _function((t - tMin) / (tMax - tMin)) * (vMax - vMin) + vMin;
	}

public:
	static const Transition LINEAR;
	static const Transition QUADRATIC_IN;
	static const Transition QUADRATIC_OUT;
	static const Transition CUBIC;
	static const Transition CUBIC_IN;
	static const Transition CUBIC_OUT;
	static const Transition COSINE;

};

}

#endif
