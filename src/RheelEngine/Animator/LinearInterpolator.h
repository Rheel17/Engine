/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_LINEARINTERPOLATOR_H
#define RHEELENGINE_LINEARINTERPOLATOR_H
#include "../_common.h"

#include "TransitionInterpolator.h"

namespace rheel {

template<typename T>
class RE_API LinearInterpolator : public TransitionInterpolator<T> {

public:
	LinearInterpolator() :
			TransitionInterpolator<T>(Transition::linear) {}

};

}

#endif
