/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_LINEARINTERPOLATOR_H
#define RHEELENGINE_LINEARINTERPOLATOR_H
#include "../_common.h"

#include "TransitionInterpolator.h"

namespace rheel {

template<typename V>
class LinearInterpolator : public TransitionInterpolator<V> {

public:
	LinearInterpolator() :
			TransitionInterpolator<V>(Transition::LINEAR) {}

};

}

#endif
