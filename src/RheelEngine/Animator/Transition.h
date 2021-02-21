/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_TRANSITION_H
#define RHEELENGINE_TRANSITION_H
#include "../_common.h"

namespace rheel {

class RE_API Transition {

public:
	template<typename V>
	V operator()(float t, float t_min, float t_max, V v_min, V v_max) const {
		return _function((t - t_min) / (t_max - t_min)) * (v_max - v_min) + v_min;
	}

private:
	using TransitionFunction = float (*)(float);

	explicit Transition(TransitionFunction function);

	TransitionFunction _function;

public:
	static const Transition linear;
	static const Transition quadratic_in;
	static const Transition quadratic_out;
	static const Transition cubic;
	static const Transition cubic_in;
	static const Transition cubic_out;
	static const Transition cosine;

};

}

#endif
