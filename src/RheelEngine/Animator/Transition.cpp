/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Transition.h"

namespace rheel {

static float transition_linear(float t) { return t; }
static float transition_quadratic_in(float t) { return t * t; }
static float transition_quadratic_out(float t) { return 1.0f - (t - 1.0f) * (t - 1.0f); }
static float transition_cubic(float t) { return -2.0f * t * t * t + 3.0f * t * t; }
static float transition_cubic_in(float t) { return t * t * t; }
static float transition_cubic_out(float t) { return t * t * t - 3 * t * t + 3 * t; }
static float transition_cosine(float t) { return (1.0f - static_cast<float>(std::cos(M_PI * t))) / 2.0f; }

const Transition Transition::linear(transition_linear);
const Transition Transition::quadratic_in(transition_quadratic_in);
const Transition Transition::quadratic_out(transition_quadratic_out);
const Transition Transition::cubic(transition_cubic);
const Transition Transition::cubic_in(transition_cubic_in);
const Transition Transition::cubic_out(transition_cubic_out);
const Transition Transition::cosine(transition_cosine);

Transition::Transition(Transition::TransitionFunction function) :
		_function(function) {}

}