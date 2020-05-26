/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Transition.h"

namespace rheel {

static float transitionLinear(float t) { return t; }
static float transitionQuadraticIn(float t) { return t * t; }
static float transitionQuadraticOut(float t) { return 1.0f - (t - 1.0f) * (t - 1.0f); }
static float transitionCubic(float t) { return -2.0f * t * t * t + 3.0f * t * t; }
static float transitionCubicIn(float t) { return t * t * t; }
static float transitionCubicOut(float t) { return t * t * t - 3 * t * t + 3 * t; }
static float transitionCosine(float t) { return (1.0f - static_cast<float>(std::cos(M_PI * t))) / 2.0f; }

const Transition Transition::LINEAR(transitionLinear);
const Transition Transition::QUADRATIC_IN(transitionQuadraticIn);
const Transition Transition::QUADRATIC_OUT(transitionQuadraticOut);
const Transition Transition::CUBIC(transitionCubic);
const Transition Transition::CUBIC_IN(transitionCubicIn);
const Transition Transition::CUBIC_OUT(transitionCubicOut);
const Transition Transition::COSINE(transitionCosine);

Transition::Transition(Transition::TransitionFunction function) :
		_function(function) {}

}