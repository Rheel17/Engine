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
	float operator()(float t) const;
	float operator()(float t, float tMin, float tMax, float vMin, float vMax) const;

public:
	static const Transition linear;
	static const Transition quadratic_in;
	static const Transition quadratic_out;
	static const Transition cubic;
	static const Transition cosine;

};

}

#endif
