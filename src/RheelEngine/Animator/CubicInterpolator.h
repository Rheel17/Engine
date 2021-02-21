/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CUBICINTERPOLATOR_H
#define RHEELENGINE_CUBICINTERPOLATOR_H
#include "../_common.h"

#include "TransitionInterpolator.h"

namespace rheel {

template<typename V>
class CubicInterpolator : public TransitionInterpolator<V> {

public:
	CubicInterpolator() :
			TransitionInterpolator<V>(Transition::cubic) {}

};

}

#endif
