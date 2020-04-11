/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Transition.h"

namespace rheel {

static float transitionLinear(float t) { return t; }
static float transitionQuadraticIn(float t) { return t * t; }
static float transitionQuadraticOut(float t) { return 1.0f - (t - 1.0f) * (t - 1.0f); }
static float transitionCubic(float t) {	return -2.0f * t * t * t + 3.0f * t * t; }
static float transitionCosine(float t) { return (1.0f - std::cos(M_PI * t)) / 2.0f; }

const Transition Transition::linear(transitionLinear);
const Transition Transition::quadratic_in(transitionQuadraticIn);
const Transition Transition::quadratic_out(transitionQuadraticOut);
const Transition Transition::cubic(transitionCubic);
const Transition Transition::cosine(transitionCosine);

Transition::Transition(Transition::_TransitionFunction function) :
		_function(function) {}

float Transition::operator()(float t) const {
	return _function(t);
}

float Transition::operator()(float t, float tMin, float tMax, float vMin, float vMax) const {
	return _function((t - tMin) / (tMax - tMin)) * (vMax - vMin) + vMin;
}

}